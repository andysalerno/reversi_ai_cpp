#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include <string>
#include "coord.hpp"

enum Piece
{
    empty,
    black,
    white
};

class board
{
    size_t size;
    unsigned amount_white_pieces, amount_black_pieces;
    std::vector<std::vector<Piece>> board_vec;

  public:
    board(size_t size);
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
    inline bool operator==(const board &other);
    const std::vector<std::vector<Piece>> &get_board_vec() const
    {
        return this->board_vec;
    }
};

inline bool board::operator==(const board &other)
{
    return this->board_vec == other.board_vec;
}

namespace std
{
template <>
struct hash<std::vector<std::vector<Piece>>>
{
    std::size_t operator()(std::vector<std::vector<Piece>> const &board_vec) const
    {
        return 7;
    }
};

template <>
struct hash<board>
{
    std::size_t operator()(board const &_board) const
    {
        return std::hash<std::vector<std::vector<Piece>>>{}(_board.get_board_vec());
    }
};
}

#endif
