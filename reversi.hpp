#ifndef REVERSI_H
#define REVERSI_H

#include "agent.hpp"
#include "board.hpp"
#include <memory>
#include <vector>

void initialize_reversi_board(Board&);
Piece play_game(Board&, Agent&, Agent&, Piece, bool silent = false);
bool is_legal_move(Coord&, const Board&, Piece);
std::vector<Coord> legal_moves(const Board&, Piece);
Piece opponent(Piece);
bool is_game_over(const Board& _board);
bool apply_move(Board& board, Piece player_color, const Coord& move);
bool is_direction_valid_move(const Board& _board, const Coord& move, Piece player_color, int dx, int dy);
Piece most_pieces(const Board&);

#endif