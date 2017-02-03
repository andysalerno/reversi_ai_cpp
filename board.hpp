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
};

#endif
