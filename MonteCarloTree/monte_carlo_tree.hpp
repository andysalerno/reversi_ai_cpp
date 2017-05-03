#ifndef MONTE_CARLO_TREE_H
#define MONTE_CARLO_TREE_H

#include "../gamestate.hpp"
#include "node.hpp"
#include <functional>
#include <memory>
#include <unordered_map>

template <typename S, typename A>
class MonteCarloTree {
    using Node = Node<S, A>;
    using StateRef = std::reference_wrapper<const S>;
    using NodeRef = std::reference_wrapper<Node>;

    std::unordered_map<StateRef, NodeRef, GameStateHash, GameStateRefEqualTo> state_to_node;
    std::unique_ptr<Node> root;

public:
    Node* get_existing_node(const S& game_state)
    {
        auto check = state_to_node.find(std::cref(game_state));
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

        state_to_node.insert({ std::cref(added_child.get_game_state()), std::ref(added_child) });
        return added_child;
    }

    Node& add_root_node(S&& game_state)
    {
        this->root = std::make_unique<Node>(std::move(game_state));
        state_to_node.insert({ std::cref(this->root->get_game_state()), std::ref(*(this->root)) });
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
        for (const auto& child : parent_ptr->get_children()) {
            if (child.get() == this->root.get()) {
                this->root.reset(child.get());
            }
            break;
        }
    }
};

#endif