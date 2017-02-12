#include "tree_manager.hpp"

TreeManager::TreeManager()
    : state_node(std::unordered_map<std::shared_ptr<GameState>, std::shared_ptr<Node> >{})
{
}

std::shared_ptr<Node> TreeManager::add_node(const GameState& _game_state, const coord& coord, const Node& parent)
{
    return std::shared_ptr<Node>{};
}

std::shared_ptr<Node> TreeManager::add_node(const GameState& _game_state, const coord& coord)
{
    return std::shared_ptr<Node>{};
}

std::shared_ptr<Node> TreeManager::get_node(const GameState& _game_state)
{
    // auto search = this->state_node.find(std::shared_ptr<game_state> ptr{ &_game_state });
}