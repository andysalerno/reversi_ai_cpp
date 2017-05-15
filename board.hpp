#ifndef BOARD_HPP
#define BOARD_HPP

#include "coord.hpp"
#include <iostream>
#include <string>
#include <vector>

enum Piece {
    empty,
    black,
    white
};

class Board {
    using PieceVec = std::vector<std::vector<Piece>>;
    size_t size = 8;
    PieceVec board_vec = PieceVec{ size, std::vector<Piece>(size, empty) };
    unsigned amount_black_pieces;
    unsigned amount_white_pieces = 0;

public:
    Board();
    Board(const Board&) = default;
    Board(Board&&) = default;
    Board& operator=(const Board& other) = default;
    inline bool operator==(const Board& other) const;

    void flip_piece(Coord);
    void set_piece(Coord, Piece);
    void clear();
    bool is_full() const;
    bool is_in_bounds(Coord) const;
    unsigned get_amount_white() const;
    unsigned get_amount_black() const;
    Piece get_piece(Coord) const;
    size_t get_size() const;
    std::string stringify();
    const PieceVec& get_board_vec() const
    {
        return this->board_vec;
    }
};

inline bool Board::operator==(const Board& other) const
{
    return this->board_vec == other.board_vec;
}

namespace std {
template <>
struct hash<std::vector<std::vector<Piece>>> {
    std::size_t operator()(std::vector<std::vector<Piece>> const& board_vec) const
    {
        size_t hashval = 5138;

        for (const auto& row : board_vec) {
            for (const auto& val : row) {
                hashval += val;
                hashval += (hashval << 10);
                hashval ^= (hashval >> 6);
            }
        }

        hashval += (hashval << 3);
        hashval ^= (hashval >> 11);
        hashval += (hashval << 15);

        return hashval;
    }
};

template <>
struct hash<Board> {
    std::size_t operator()(const Board& _board) const
    {
        return std::hash<std::vector<std::vector<Piece>>>{}(_board.get_board_vec());
    }
};
}

#endif
