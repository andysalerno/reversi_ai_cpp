#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "board.hpp"

class GameState {
    Board board;
    Piece player_turn;

public:
    GameState(GameState&& other)
        : board(std::move(other.board))
        , player_turn(other.player_turn)
    {
    }

    Board* board_ptr()
    {
        return &this->board;
    }

    Piece get_player_turn()
    {
        return this->player_turn;
    }
};

#endif