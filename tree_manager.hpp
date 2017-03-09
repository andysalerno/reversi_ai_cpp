#ifndef TREE_MANAGER_H
#define TREE_MANAGER_H

#include "MonteCarloTree/node.hpp"
#include "gamestate.hpp"
#include <memory>

using ReversiNode = Node<GameState, coord>;

class TreeManager {
    std::shared_ptr<ReversiNode> root;

public:
    std::shared_ptr<ReversiNode> add_node(GameState&& game_state, coord move, ReversiNode& parent)
    {
        auto node_ptr = std::make_shared<ReversiNode>(std::move(game_state), move, &parent);
        parent.add_child(node_ptr);
        return node_ptr;
    }

    std::shared_ptr<ReversiNode> add_root_node(GameState&& game_state)
    {
        this->root = std::make_shared<ReversiNode>(std::move(game_state));
        return this->root;
    }

    std::shared_ptr<ReversiNode> add_root_node(const GameState& game_state)
    {
        GameState copy = game_state;
        this->root = std::make_shared<ReversiNode>(std::move(copy));
        return this->root;
    }
};

#endif