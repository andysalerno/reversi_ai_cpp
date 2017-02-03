#ifndef REVERSI_H
#define REVERSI_H

#include "board.hpp"
#include <vector>

void initialize_reversi_board(board &);
void play_game(board &);
bool is_legal_move(coord &, board &, Piece);
std::vector<coord> legal_moves(board &, Piece);
Piece opponent(Piece);
bool is_game_over(board &_board);

#endif