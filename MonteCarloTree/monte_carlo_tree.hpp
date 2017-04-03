#ifndef MONTE_CARLO_TREE_H
#define MONTE_CARLO_TREE_H

#include "node.hpp"
#include <functional>
#include <unordered_map>

template <typename S, typename A>
class MonteCarloTree {
    //std::unordered_map<std::reference_wrapper<const S>, std::shared_ptr<Node<S, A> > > state_to_node;

public:
    // std::shared_ptr<Node<S, A> > get_existing_node(const S& game_state)
    // {
    //     auto check = state_to_node.find(std::reference_wrapper<const S>(game_state));
    //     if (check == state_to_node.end()) {
    //         // didn't exist in memoization
    //         return std::shared_ptr<Node<S, A> >(nullptr);
    //     }

    //     auto node_ptr = check->second;
    //     return node_ptr;
    // }

    std::shared_ptr<Node<S, A> > add_node(S&& game_state, A action, Node<S, A>& parent)
    {
        auto node_ptr = std::make_shared<Node<S, A> >(std::move(game_state), action, &parent);
        parent.add_child(node_ptr);

        //state_to_node[std::reference_wrapper<const S>{node_ptr->get_game_state()}] = node_ptr;
        return node_ptr;
    }

    std::shared_ptr<Node<S, A> > add_root_node(S&& game_state)
    {
        auto node_ptr = std::make_shared<Node<S, A> >(std::move(game_state));
        //state_to_node[std::reference_wrapper<const S>{this->root->get_game_state()}] = this->root;
        return node_ptr;
    }

    std::shared_ptr<Node<S, A> > add_root_node(const S& game_state)
    {
        S copy = game_state;
        auto node_ptr = std::make_shared<Node<S, A> >(std::move(copy));
        //state_to_node[std::reference_wrapper<const S>{this->root->get_game_state()}] = this->root;
        return node_ptr;
    }
};

#endif