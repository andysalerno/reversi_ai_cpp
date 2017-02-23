#ifndef TREE_MANAGER_H
#define TREE_MANAGER_H

#include "gamestate.hpp"
#include "node.hpp"
#include <memory>

class TreeManager {
    std::shared_ptr<Node> root;

public:
    void add_node(GameState&& game_state, Node& parent)
    {
        auto node_ptr = std::make_shared<Node>(std::move(game_state), &parent);
        parent.add_child(node_ptr);
    }

    void add_root_node(GameState&& game_state)
    {
        this->root = std::make_shared<Node>(std::move(game_state));
    }
};

#endif