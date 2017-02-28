#include "monte_carlo_agent.hpp"
#include "reversi.hpp"
#include "tree_manager.hpp"

coord MonteCarloAgent::pick_move(const GameState& game_state)
{
    return this->monte_carlo_tree_search(game_state);
}

coord MonteCarloAgent::monte_carlo_tree_search(const GameState& game_state)
{
    TreeManager tree_manager{};
    auto tree_root_ptr = tree_manager.add_root_node(game_state);

    for (unsigned simulation_num = 0; simulation_num < 5000; ++simulation_num) {
        auto selected_node_ptr = this->tree_policy(tree_root_ptr, tree_manager);
        int result = this->simulate(selected_node_ptr);
        this->back_propagate(selected_node_ptr, result);
    }

    return this->best_child(tree_root_ptr)->get_move();
}

std::shared_ptr<Node> MonteCarloAgent::tree_policy(std::shared_ptr<Node> node_ptr, TreeManager& tree_manager)
{
    if (node_ptr->get_game_state().get_legal_moves().size() == 0) {
        if (is_game_over(node_ptr->get_game_state().get_board())) {
            // no available moves + game is over
            return node_ptr;
        } else {
            // no available moves + game is NOT over, so must pass turn
            // passing a turn acts as a "move" and gets its own node
            const Board& pass_board = node_ptr->get_game_state().get_board();
            auto pass_legal_moves = legal_moves(pass_board, opponent(this->color));
            GameState pass_state{ pass_board, pass_legal_moves, opponent(this->color) };
            auto pass_node = tree_manager.add_node(std::move(pass_state), { 0, 0 }, *node_ptr); // TODO: move coord should not be {0, 0}
            return pass_node;
        }
    }
    else
    {
        // this node has children to select from
        // see if any children have not been played yet
        for (const auto& child : node_ptr->get_children())
        {
            if (child->get_plays() == 0)
            {
                return child;
            }
        }

        // there was not an unplayed child, so pick one with UCB
        return this->best_child(node_ptr);
    }
}

void MonteCarloAgent::back_propagate(std::shared_ptr<Node> node_ptr, int result)
{
}

int MonteCarloAgent::simulate(std::shared_ptr<Node> node_ptr)
{
}

std::shared_ptr<Node> MonteCarloAgent::best_child(std::shared_ptr<Node> root_ptr)
{
    const bool is_enemy_turn = root_ptr->get_game_state().get_player_turn() != this->color;
    const int C = 1; // exploration value
    std::vector<std::pair<Node *, int>> node_scores;
    unsigned parent_plays = root_ptr->get_plays();
}