#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "board.hpp"
#include <vector>

class GameState {
    Board board;
    Piece player_turn;
    std::vector<coord> legal_moves;

public:
    GameState(GameState&& other)
        : board(std::move(other.board))
        , player_turn(other.player_turn)
        , legal_moves(std::move(other.legal_moves))
    {
    }

    GameState(const GameState& other)
        : board(other.board)
        , player_turn(other.player_turn)
        , legal_moves(other.legal_moves)
    {
    }

    GameState(const Board& _board, const std::vector<coord>& _legal_moves, Piece _player_turn)
        : board(_board)
        , player_turn(_player_turn)
        , legal_moves(_legal_moves)
    {
    }

    GameState(const Board& _board, std::vector<coord>&& _legal_moves, Piece _player_turn)
        : board(_board)
        , player_turn(_player_turn)
        , legal_moves(std::move(_legal_moves))
    {
    }

    const Board& get_board() const
    {
        return this->board;
    }

    Piece get_player_turn() const
    {
        return this->player_turn;
    }

    const auto& get_legal_moves() const
    {
        return this->legal_moves;
    }
};

#endif