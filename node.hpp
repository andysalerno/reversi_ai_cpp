#ifndef NODE_H
#define NODE_H

#include "gamestate.hpp"
#include <memory>
#include <vector>

class Node {
    GameState game_state;
    unsigned wins;
    unsigned plays;
    Node* parent;
    std::vector<std::shared_ptr<Node> > children;

public:
    Node(GameState&& _game_state)
        : game_state(std::move(_game_state))
        , wins(0)
        , plays(0)
        , parent(nullptr)
        , children(std::vector<std::shared_ptr<Node> >{})
    {
    }

    Node(GameState&& _game_state, Node* _parent)
        : game_state(std::move(_game_state))
        , wins(0)
        , plays(0)
        , parent(_parent)
        , children(std::vector<std::shared_ptr<Node> >{})
    {
    }

    void add_child(std::shared_ptr<Node> child)
    {
        this->children.push_back(child);
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
};

#endif