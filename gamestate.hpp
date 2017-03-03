#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "board.hpp"
#include <vector>
#include <boost/functional/hash.hpp>

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

    GameState(Board _board, std::vector<coord> _legal_moves, Piece _player_turn)
        : board(_board)
        , player_turn(_player_turn)
        , legal_moves(_legal_moves)
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

struct GameStateHasher
{
    std::size_t operator(const GameState& a, const GameState& b) const
    {


    }
};

#endif