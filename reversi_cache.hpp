#ifndef REVERSI_CACHE
#define REVERSI_CACHE

#include "coord.hpp"
#include <utility>
#include <vector>

class ReversiCache {
    std::pair<GameState, std::vector<Coord> > A;
    std::pair<GameState, std::vector<Coord> > B;
    std::pair<GameState, std::vector<Coord> > C;

    decltype(&A) rolling_ptr = &A;

public:
    void insert(Board _board, Piece color, std::vector<Coord> legal_moves)
    {
        GameState game_state(_board, legal_moves, color);
        *rolling_ptr = std::make_pair(std::move(game_state), legal_moves);

        if (rolling_ptr == &A) {
            rolling_ptr = &B;
        } else if (rolling_ptr == &B) {
            rolling_ptr = &C;
        } else {
            rolling_ptr = &A;
        }
    }

    bool try_query_cache(const Board& _board, Piece color, std::vector<Coord>& result) const
    {
        if (A.first.get_player_turn() == color && A.first.get_board() == _board) {
            result = A.second;
            return true;
        } else if (B.first.get_player_turn() == color && B.first.get_board() == _board) {
            result = B.second;
            return true;
        } else if (C.first.get_player_turn() == color && C.first.get_board() == _board) {
            result = C.second;
            return true;
        } else {
            return false;
        }
    }
};

#endif