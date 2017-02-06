#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "board.hpp"

struct game_state
{
    board &_board;
    Piece player_turn;
};

bool operator==(const game_state &lhs, const game_state &rhs)
{
    return lhs._board == rhs._board && lhs.player_turn == rhs.player_turn;
}

namespace std
{
template <>
struct hash<game_state>
{
    std::size_t operator()(game_state const &gs) const
    {
        std::size_t h1 = std::hash<board>{}(gs._board);
        std::size_t h2 = std::hash<Piece>{}(gs.player_turn);
        return h1 ^ (h2 << 1);
    }
};
}

#endif