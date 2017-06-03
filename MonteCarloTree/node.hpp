#ifndef NODE_H
#define NODE_H

#include "../util.hpp"
#include <memory>
#include <utility>
#include <vector>

template <typename S, typename A>
class Node {
    unsigned plays = 0;
    unsigned wins = 0;
    S game_state;
    A action = A{};

    Node* parent = nullptr;
    std::vector<std::unique_ptr<Node>> children;

public:
    Node(S&& _game_state, A _action = A{}, Node* _parent = nullptr)
        : game_state(std::forward<S>(_game_state))
        , action(_action)
        , parent(_parent)
    {
    }

    Node(Node&&) noexcept = default;
    Node(){};
    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;
    Node& operator=(Node&&) = default;

    std::function<void()> fire_on_destruct = []() -> void { return; };

    void set_destructor(std::function<void()> func)
    {
        this->fire_on_destruct = func;
    }

    ~Node()
    {
        this->fire_on_destruct();
    }

    Node& add_child(Node&& child)
    {
        this->children.push_back(std::make_unique<Node>(std::move(child)));
        return *(this->children.back());
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

    Node* get_parent() const
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