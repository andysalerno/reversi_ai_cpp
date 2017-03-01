#include "monte_carlo_agent.hpp"
#include "random_agent.hpp"
#include "reversi.hpp"
#include "tree_manager.hpp"
#include "util.hpp"
#include <cmath>
#include <limits>

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
        unsigned result = this->simulate(selected_node_ptr);
        this->back_propagate(selected_node_ptr, result);
    }

    return this->best_child(tree_root_ptr)->get_move();
}

std::shared_ptr<Node> MonteCarloAgent::tree_policy(std::shared_ptr<Node> node_ptr, TreeManager& tree_manager)
{
    const GameState& game_state = node_ptr->get_game_state();
    const Board& board = game_state.get_board();
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
    } else {
        // this node has children to select from

        auto legal_moves = game_state.get_legal_moves();
        if (legal_moves.size() > node_ptr->get_children().size()) {
            // there are more possible child states than there are currently child nodes
            // so we must be able to create new child nodes for these states

            // find a move that does not have a node yet, and create a node for it
            const auto& child_nodes = node_ptr->get_children();
            auto exists_child_with_move = [child_nodes](auto& move) { return false; };
            for (const auto& move : legal_moves) {
                if (!exists_child_with_move(move)) {
                    // we have found a move that does not have a node yet, so create it
                    Board next_board{ board };
                    Piece color = game_state.get_player_turn();
                    apply_move(next_board, color, move);
                    auto next_moves = legal_moves(next_board, opponent(color));
                    GameState next_state{ next_board, next_moves, opponent(color) };
                    auto next_node = tree_manager.add_node(std::move(next_state), move, node_ptr.get());
                    return next_node;
                }
            }
        }

        // there was not an unplayed child, so recurse with UCB
        return this->tree_policy(best_child(node_ptr));
    }
}

void MonteCarloAgent::back_propagate(std::shared_ptr<Node> node_ptr, unsigned result)
{
    Node* parent_ptr = node_ptr->get_parent();
    while (parent_ptr != nullptr) {
        parent_ptr->increment_plays();
        parent_ptr->update_wins(result);

        parent_ptr = parent_ptr->get_parent();
    }
}

unsigned MonteCarloAgent::simulate(std::shared_ptr<Node> node_ptr)
{
    const unsigned WIN_RESULT = 1;
    const unsigned LOSS_RESULT = 0;

    // play a random game
    Board board_copy{ node_ptr->get_game_state().get_board() };
    auto random_agent_black = std::unique_ptr<agent>(new random_agent{ black });
    auto random_agent_white = std::unique_ptr<agent>(new random_agent{ white });

    Piece winner = play_game(board_copy, random_agent_black, random_agent_white, node_ptr->get_game_state().get_player_turn(), true);

    if (winner == this->color) {
        return WIN_RESULT;
    } else {
        return LOSS_RESULT;
    }
}

std::shared_ptr<Node> MonteCarloAgent::best_child(std::shared_ptr<Node> root_ptr)
{
    const bool is_enemy_turn = root_ptr->get_game_state().get_player_turn() != this->color;
    const double C = 1; // exploration value
    std::vector<std::pair<std::shared_ptr<Node>, double> > node_scores;
    unsigned parent_plays = root_ptr->get_plays();

    for (const auto& child_ptr : root_ptr->get_children()) {
        unsigned wins = child_ptr->get_wins();
        unsigned plays = child_ptr->get_plays();

        if (is_enemy_turn) {
            wins = plays - wins;
        }

        double score = (wins / (double)plays) + C * std::sqrt(2 * std::log(parent_plays) / (double)plays);
        auto node_score = std::make_pair(child_ptr, score);
        node_scores.push_back(node_score);
    }

    double best_score = std::numeric_limits<double>::lowest();
    std::shared_ptr<Node> best_node_ptr;
    for (const auto& child_score_pair : node_scores) {
        double score = std::get<double>(child_score_pair);
        if (score > best_score) {
            best_score = score;
            best_node_ptr = std::get<std::shared_ptr<Node> >(child_score_pair);
        }
    }

    return best_node_ptr;
}