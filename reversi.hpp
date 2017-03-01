#ifndef REVERSI_H
#define REVERSI_H

#include "agent.hpp"
#include "board.hpp"
#include <memory>
#include <vector>

void initialize_reversi_board(Board&);
Piece play_game(Board&, std::unique_ptr<agent>&, std::unique_ptr<agent>&, Piece, bool silent = false);
bool is_legal_move(coord&, const Board&, Piece);
std::vector<coord> legal_moves(const Board&, Piece);
Piece opponent(Piece);
bool is_game_over(const Board& _board);
bool apply_move(Board& board, Piece player_color, const coord& move);
bool is_direction_valid_move(const Board& _board, const coord& move, Piece player_color, int dx, int dy);
Piece most_pieces(const Board&);

#endif