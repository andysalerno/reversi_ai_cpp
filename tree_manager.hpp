#ifndef TREE_MANAGER_H
#define TREE_MANAGER_H

#include "board.hpp"
#include <memory>
#include <unordered_map>

struct GameState {
    Board _board;
    Piece player_turn;
    inline bool operator==(const GameState&);
};

inline bool GameState::operator==(const GameState& other)
{
    return this->_board == other._board && this->player_turn == other.player_turn;
}

class Node {
    unsigned plays;
    unsigned wins;
    std::weak_ptr<Node> parent;
    std::vector<std::shared_ptr<Node> > children;
    GameState& game_state;
};

namespace std {
template <>
struct hash<GameState> {
    std::size_t operator()(GameState const& gs) const
    {
        std::size_t h1 = std::hash<Board>{}(gs._board);
        std::size_t h2 = std::hash<Piece>{}(gs.player_turn);
        return h1 ^ (h2 << 1);
    }
};
}

class TreeManager {
    std::unordered_map<std::shared_ptr<GameState>, std::shared_ptr<Node> >
        state_node;

public:
    TreeManager();
    std::shared_ptr<Node> add_node(const GameState&, const coord&, const Node&);
    std::shared_ptr<Node> add_node(const GameState&, const coord&);
    std::shared_ptr<Node> get_node(const GameState&);
};

#endif