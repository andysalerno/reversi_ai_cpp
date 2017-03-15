#ifndef BOARD_HPP
#define BOARD_HPP

#include "coord.hpp"
#include <string>
#include <vector>

enum Piece {
    empty,
    black,
    white
};

class Board {
    size_t size = 8;
    std::vector<std::vector<Piece>> board_vec;
    unsigned amount_black_pieces, amount_white_pieces;

public:
    Board();
    Board(Board&&);
    Board(const Board&);

    void flip_piece(coord);
    void set_piece(coord, Piece);
    void clear();
    unsigned get_amount_white() const;
    unsigned get_amount_black() const;
    bool is_full() const;
    Piece get_piece(coord) const;
    bool is_in_bounds(coord) const;
    size_t get_size() const;
    std::string stringify();
    inline bool operator==(const Board& other) const;
    Board& operator=(const Board& other) = default;
    const std::vector<std::vector<Piece>>& get_board_vec() const
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
        int hashval = 5138;

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
