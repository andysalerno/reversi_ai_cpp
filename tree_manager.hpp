#ifndef TREE_MANAGER_H
#define TREE_MANAGER_H

#include "gamestate.hpp"
#include "node.hpp"
#include <memory>
#include <unordered_map>

class TreeManager {
    std::shared_ptr<Node> root;
    std::unordered_map<GameState*, Node*> state_to_node{};

public:
    std::shared_ptr<Node> add_node(GameState&& game_state, coord move, Node& parent)
    {
        auto node_ptr = std::make_shared<Node>(std::move(game_state), move, &parent);
        parent.add_child(node_ptr);
        return node_ptr;
    }

    std::shared_ptr<Node> add_root_node(GameState&& game_state)
    {
        this->root = std::make_shared<Node>(std::move(game_state));
        return this->root;
    }

    std::shared_ptr<Node> add_root_node(const GameState& game_state)
    {
        GameState copy = game_state;
        this->root = std::make_shared<Node>(std::move(copy));
        return this->root;
    }
};

#endif