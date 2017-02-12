#include "monte_carlo_agent.hpp"
#include "board.hpp"
#include "coord.hpp"
#include "reversi.hpp"
#include "util.hpp"
#include <utility>
#include <vector>

monte_carlo_agent::monte_carlo_agent(Piece _color)
    : agent(_color)
{
}

coord monte_carlo_agent::pick_move(const Board& board, std::vector<coord>& legal_moves)
{
    return this->mcts(GameState(board, legal_moves, this->color), legal_moves);
}

coord monte_carlo_agent::mcts(GameState&& game_state, std::vector<coord>& legal_moves)
{
    auto root_ptr = tree_manager.add_root_node(std::move(game_state));

    size_t sim_count = 0;
    while (sim_count < 1000) {
        auto picked_node = this->tree_policy(root_ptr);
        int sim_result = this->simulate(picked_node->game_state);
        this->back_prop(*picked_node, sim_result);
        ++sim_count;
    }
}

std::shared_ptr<Node> monte_carlo_agent::tree_policy(std::shared_ptr<Node> node_ptr)
{
    auto& current_legal_moves = node_ptr->game_state.legal_moves;
    if (current_legal_moves.size() == 0) {
        // TODO: what if it's a pass turn?
        return node_ptr;

        // Otherwise we do have legal moves, so pick one
    } else if (node_ptr->children.size() < current_legal_moves.size()) {
        // This node has unexplored children; prioritize visiting them
        // at least once before trying any visited children
        for (const auto& move : current_legal_moves) {
            auto untried_move = node_ptr->moves_tried.find(move);
            if (untried_move == node_ptr->moves_tried.end()) {
                // move wasn't found in moves_tried, therefore we haven't try it, so let's try it
                Board board = node_ptr->game_state.board;
                apply_move(board, *untried_move, this->color);
                auto next_legal_moves = legal_moves(board, opponent(this->color));
                GameState game_state(board, next_legal_moves, opponent(this->color)); // TODO: move board, don't copy, same with legal_moves

                node_ptr->moves_tried.insert(*untried_move);
                return this->tree_manager.add_node(std::move(game_state), *untried_move, *node_ptr);
            }
        }
    } else {
        return this->tree_policy(this->best_child(node_ptr));
    }
}

int monte_carlo_agent::simulate(GameState& game_state)
{
    return 42;
}
std::shared_ptr<Node> monte_carlo_agent::best_child(std::shared_ptr<Node> node_ptr)
{
    const bool is_enemy_turn = node_ptr->game_state.player_turn == opponent(this->color);
    const int C = 1; // 'exploration' value
    //unsigned parent_plays =
}

void monte_carlo_agent::back_prop(Node& node, int result)
{
}