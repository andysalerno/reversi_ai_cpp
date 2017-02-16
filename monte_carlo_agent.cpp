#include "monte_carlo_agent.hpp"
#include "board.hpp"
#include "coord.hpp"
#include "reversi.hpp"
#include "util.hpp"
#include <limits>
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

    // return the child node with the most plays
    coord* best = nullptr;
    unsigned most_plays = 0;
    for (const auto& child : root_ptr->children) {
        if (child->plays > most_plays) {
            best = &child->move;
            most_plays = child->plays;
        }
    }

    return *best;
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
            auto move_search = node_ptr->moves_tried.find(move);
            if (move_search == node_ptr->moves_tried.end()) {
                // move wasn't found in moves_tried, therefore we haven't try it, so let's try it
                Board board = node_ptr->game_state.board;
                apply_move(board, move, this->color);
                auto next_legal_moves = legal_moves(board, opponent(this->color));
                GameState game_state(board, next_legal_moves, opponent(this->color)); // TODO: move board, don't copy, same with legal_moves

                node_ptr->moves_tried.insert(move);
                return this->tree_manager.add_node(std::move(game_state), move, node_ptr.get());
            }
        }
    } else {
        return this->tree_policy(this->best_child(node_ptr));
    }
}

int monte_carlo_agent::simulate(GameState game_state)
{
    const int WIN_VALUE = 1;
    const int LOSS_VALUE = 0;
    Piece player_turn = game_state.player_turn;

    while (!is_game_over(game_state.board)) {
        auto random_move = vec_pick_random(game_state.legal_moves);
        apply_move(game_state.board, random_move, player_turn);
        player_turn = opponent(player_turn);
    }

    Piece _winner = winner(game_state.board);
    if (_winner == this->color) {
        return WIN_VALUE;
    } else {
        return LOSS_VALUE;
    }
}
std::shared_ptr<Node> monte_carlo_agent::best_child(std::shared_ptr<Node> node_ptr)
{
    const bool is_enemy_turn = node_ptr->game_state.player_turn == opponent(this->color);
    const int C = 1; // 'exploration' value
    unsigned parent_plays = node_ptr->plays;

    double best_score = -std::numeric_limits<double>::infinity();
    std::shared_ptr<Node> best_node{ nullptr };
    for (const auto& child : node_ptr->children) {
        unsigned wins = child->wins;
        unsigned plays = child->plays;
        if (is_enemy_turn) {
            wins = plays - wins;
        }
        double score = (wins / plays) + C * std::sqrt(2 * std::log(parent_plays) / plays);
        if (score >= best_score) {
            best_score = score;
            best_node = child;
        }
    }

    return best_node;
}

void monte_carlo_agent::back_prop(Node& node, int result)
{
    Node* node_ptr = &node;
    while (node_ptr != nullptr) {
        node_ptr->plays++; // note: ...don't do this here do it in visit?
        node_ptr->wins += result;
        node_ptr = node_ptr->parent;
    }
}