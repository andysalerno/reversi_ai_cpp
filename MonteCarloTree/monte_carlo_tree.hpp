#ifndef MONTE_CARLO_TREE_H
#define MONTE_CARLO_TREE_H

#include "../gamestate.hpp"
#include "node.hpp"
#include <functional>
#include <memory>
#include <unordered_map>
#include <utility>

template <typename S, typename A>
class MonteCarloTree {
    using Node = Node<S, A>;
    using NodeRef = std::reference_wrapper<Node>;

    std::unordered_map<S, NodeRef, GameStateHash> state_to_node;
    std::unique_ptr<Node> root;

public:
    Node* get_existing_node(const S& game_state)
    {
        auto check = state_to_node.find(game_state);
        if (check == state_to_node.end()) {
            return nullptr;
        }

        Node& node = check->second;
        return &node;
    }

    Node& add_node(S&& game_state, A action, Node& parent)
    {
        auto new_node = Node{ std::move(game_state), action, &parent };
        auto& added_child = parent.add_child(std::move(new_node));

        auto node_ref = std::ref(added_child);

        state_to_node.insert({ added_child.get_game_state(), node_ref });
        return added_child;
    }

    Node& add_root_node(S&& game_state)
    {
        this->root = std::make_unique<Node>(std::forward<S>(game_state));
        state_to_node.insert({ this->root->get_game_state(), std::ref(*(this->root)) });
        return *(this->root);
    }

    Node& add_root_node(const S& game_state)
    {
        S copy = game_state;
        auto& root = this->add_root_node(std::move(copy));
        return root;
    }

    void set_root(const Node& new_root)
    {
        // move unique_ptr from parent to the root_ptr
        auto parent_ptr = new_root.get_parent();
        if (parent_ptr == nullptr) {
            return;
        }

        for (const auto& child : parent_ptr->get_children()) {
            if (child.get() == &new_root) {
                this->root.reset(child.get());
                return;
            }
        }
    }
};

#endif