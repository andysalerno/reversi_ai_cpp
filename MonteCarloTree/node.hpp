#ifndef NODE_H
#define NODE_H

#include "../util.hpp"
#include <memory>
#include <vector>

template <typename S, typename A>
class Node {
    unsigned plays;
    unsigned wins;
    S game_state;
    A action;

    Node<S, A>* parent;
    std::vector<std::shared_ptr<Node<S, A>>> children;

public:
    Node(S&& _game_state, A _action = { 0, 0 }, Node* _parent = nullptr)
        : plays(0)
        , wins(0)
        , game_state(std::move(_game_state))
        , action(_action)
        , parent(_parent)
        , children(std::vector<std::shared_ptr<Node<S, A>>>{})
    {
    }

    ~Node()
    {
        //std::cout << "node destroyed.\n";
    }

    Node() = delete;
    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;

    void add_child(std::shared_ptr<Node<S, A>> child)
    {
        this->children.push_back(child);
    }

    const auto& get_children() const
    {
        return this->children;
    }

    unsigned get_wins() const
    {
        return this->wins;
    }

    unsigned get_plays() const
    {
        return this->plays;
    }

    void increment_plays()
    {
        this->plays = this->plays + 1; // (this->plays)++ ??
    }

    void update_wins(int result)
    {
        this->wins = this->wins + result;
    }

    Node<S, A>* get_parent() const
    {
        return this->parent;
    }

    const S& get_game_state() const
    {
        return this->game_state;
    }

    A get_move() const
    {
        return this->action;
    }
};

#endif