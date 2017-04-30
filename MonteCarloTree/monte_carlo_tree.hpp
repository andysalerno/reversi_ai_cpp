#ifndef MONTE_CARLO_TREE_H
#define MONTE_CARLO_TREE_H

#include "node.hpp"
#include <functional>
#include <unordered_map>

template <typename S, typename A>
class MonteCarloTree {
    using Node = Node<S, A>;
    using StateRef = std::reference_wrapper<const S>;

    std::unordered_map<StateRef, std::shared_ptr<Node> > state_to_node;
    std::shared_ptr<Node> root;

public:
    std::shared_ptr<Node> get_existing_node(const S& game_state)
    {
        auto check = state_to_node.find(StateRef(game_state));
        if (check == state_to_node.end()) {
            return std::shared_ptr<Node>(nullptr);
        }

        auto node_ptr = check->second;
        return node_ptr;
    }

    std::shared_ptr<Node> add_node(S&& game_state, A action, Node& parent)
    {
        auto node_ptr = std::make_shared<Node>(std::move(game_state), action, &parent);
        parent.add_child(node_ptr);

        state_to_node[StateRef{ node_ptr->get_game_state() }] = node_ptr;
        return node_ptr;
    }

    std::shared_ptr<Node> add_root_node(S&& game_state)
    {
        auto node_ptr = std::make_shared<Node>(std::move(game_state));
        this->root = node_ptr;
        state_to_node[StateRef{ this->root->get_game_state() }] = this->root;
        return node_ptr;
    }

    std::shared_ptr<Node> add_root_node(const S& game_state)
    {
        S copy = game_state;
        return this->add_root_node(std::move(copy));
    }
};

#endif