#ifndef NODE_H
#define NODE_H

#include "coord.hpp"
#include "gamestate.hpp"
#include <memory>
#include <vector>

class Node {
    GameState game_state;
    unsigned wins;
    unsigned plays;
    Node* parent;
    coord move;
    std::vector<std::shared_ptr<Node> > children;

public:
    Node(GameState&& _game_state, coord _move = { 0, 0 }, Node* _parent = nullptr)
        : game_state(std::move(_game_state))
        , wins(0)
        , plays(0)
        , parent(_parent)
        , move(_move)
        , children(std::vector<std::shared_ptr<Node> >{})
    {
    }

    void add_child(std::shared_ptr<Node> child)
    {
        this->children.push_back(child);
    }

    const auto& get_children()
    {
        return this->children;
    }

    unsigned get_wins()
    {
        return this->wins;
    }

    unsigned get_plays()
    {
        return this->plays;
    }

    Node* get_parent()
    {
        return this->parent;
    }

    const GameState& get_game_state()
    {
        return this->game_state;
    }

    coord get_move()
    {
        return this->move;
    }
};

#endif