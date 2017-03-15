#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "board.hpp"
#include <functional>
#include <vector>

class GameState {
    Board board;
    Piece player_turn;
    std::vector<coord> legal_moves;

public:
    GameState(const GameState&) = default;
    GameState(GameState&& other)
        : board(std::move(other.board))
        , player_turn(other.player_turn)
        , legal_moves(std::move(other.legal_moves))
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

namespace std {
template <>
struct hash<std::reference_wrapper<const GameState>> {
    std::size_t operator()(std::reference_wrapper<const GameState> const& game_state_ref) const
    {
        const GameState& game_state = game_state_ref.get();
        size_t val = std::hash<Board>{}(game_state.get_board());
        val += 17 * ((size_t)game_state.get_player_turn() + 3);
        return val;
    }
};
}

inline bool operator==(const std::reference_wrapper<const GameState>& lhs, const std::reference_wrapper<const GameState>& rhs)
{
    return lhs.get().get_board() == rhs.get().get_board()
        && lhs.get().get_player_turn() == rhs.get().get_player_turn();
}

#endif