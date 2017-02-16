#include "tree_manager.hpp"
#include <utility>

TreeManager::TreeManager()
    : state_node(std::unordered_map<std::shared_ptr<GameState>, std::shared_ptr<Node> >{})
{
}

std::shared_ptr<Node> TreeManager::add_node(GameState&& game_state, const coord& coord, Node* parent)
{
    auto node_ptr = std::make_shared<Node>(std::move(game_state), coord, parent);
    parent->add_child(node_ptr);
    return node_ptr;
}

std::shared_ptr<Node> TreeManager::add_root_node(GameState&& game_state)
{
    return std::make_shared<Node>(std::move(game_state));
}

std::shared_ptr<Node> TreeManager::get_node(GameState&& game_state)
{
    // auto search = this->state_node.find(std::shared_ptr<game_state> ptr{ &_game_state });
}