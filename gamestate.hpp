#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "board.hpp"
#include <functional>
#include <iostream>
#include <vector>

class GameState {
    Board board;
    Piece player_turn;
    std::vector<Coord> legal_moves;

public:
    GameState(const Board& _board, const std::vector<Coord>& _legal_moves, Piece _player_turn)
        : board(_board)
        , player_turn(_player_turn)
        , legal_moves(_legal_moves)
    {
    }

    GameState(const Board& _board, std::vector<Coord>&& _legal_moves, Piece _player_turn)
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

inline bool operator==(const GameState& lhs, const GameState& rhs)
{
    return lhs.get_board() == rhs.get_board()
        && lhs.get_player_turn() == rhs.get_player_turn();
}

// namespace std {
// template <>
// struct hash<std::reference_wrapper<const GameState> > {
//     std::size_t operator()(const std::reference_wrapper<const GameState>& game_state) const
//     {
//         const GameState& gs = game_state;
//         size_t val = std::hash<Board>{}(gs.get_board());
//         val += 17 * ((size_t)gs.get_player_turn() + 3);
//         return val;
//     }
// };
// }

struct GameStateRefEqualTo {
    bool operator()(const std::reference_wrapper<const GameState>& lhs,
        const std::reference_wrapper<const GameState>& rhs) const
    {
        return lhs.get() == rhs.get();
    }
};

struct GameStateHash {
    std::size_t operator()(const GameState& game_state) const
    {
        size_t val = std::hash<Board>{}(game_state.get_board());
        val += 17 * ((size_t)game_state.get_player_turn() + 3);
        return val;
    }
};

#endif