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
    unsigned amount_white_pieces, amount_black_pieces;
    std::vector<std::vector<Piece> > board_vec;

public:
    Board();
    Board(Board&&);
    Board(const Board&);

    void flip_piece(coord);
    void set_piece(coord, Piece);
    void clear();
    unsigned get_amount_white();
    unsigned get_amount_black();
    bool is_full();
    Piece get_piece(coord);
    bool is_in_bounds(coord);
    size_t get_size();
    std::string stringify();
    inline bool operator==(const Board& other);
    const std::vector<std::vector<Piece> >& get_board_vec() const
    {
        return this->board_vec;
    }
};

inline bool Board::operator==(const Board& other)
{
    return this->board_vec == other.board_vec;
}

namespace std {
template <>
struct hash<std::vector<std::vector<Piece> > > {
    std::size_t operator()(std::vector<std::vector<Piece> > const& board_vec) const
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
    std::size_t operator()(Board const& _board) const
    {
        return std::hash<std::vector<std::vector<Piece> > >{}(_board.get_board_vec());
    }
};
}

#endif
