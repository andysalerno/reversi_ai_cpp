#include "monte_carlo_agent.hpp"
#include "coord.hpp"
#include "random_agent.hpp"
#include "reversi.hpp"
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <sstream>

using Tree = MonteCarloTree<GameState, Coord>;

constexpr auto SIM_TIME_SEC = 3;

Coord MonteCarloAgent::pick_move(const GameState& game_state)
{
    return this->monte_carlo_tree_search(game_state);
}

Coord MonteCarloAgent::monte_carlo_tree_search(const GameState& game_state)
{
    ReversiNode* tree_root_ptr = this->reversi_tree.get_existing_node(game_state);

    if (tree_root_ptr == nullptr) {
        tree_root_ptr = &(this->reversi_tree.add_root_node(game_state));
    } else {
        //this->reversi_tree.set_root(*tree_root_ptr);
    }

    const auto timespan = std::chrono::seconds{ SIM_TIME_SEC };
    using clock = std::chrono::system_clock;
    auto start_time = clock::now();

    unsigned simulations = 0;
    //while (clock::now() - start_time < timespan) {
    while (simulations < 1000) {
        auto& selected_node = this->tree_policy(*tree_root_ptr);
        unsigned result = this->simulate(selected_node);
        this->back_propagate(selected_node, result);
        ++simulations;
    }

    unsigned effective_simulations = 0;
    for (const auto& child : tree_root_ptr->get_children()) {
        effective_simulations += child->get_plays();
    }

    show("\n" + this->node_scores_str(tree_root_ptr->get_children()));
    show(std::to_string(simulations) + " simulations performed. (effective: " + std::to_string(effective_simulations) + " +" + std::to_string(effective_simulations - simulations) + ")\n");
    auto& best_child = this->winningest_node(tree_root_ptr->get_children());
    return best_child.get_move();
}

ReversiNode& MonteCarloAgent::tree_policy(ReversiNode& node)
{
    const GameState& game_state = node.get_game_state();
    const Board& board = game_state.get_board();

    if (game_state.get_legal_moves().size() == 0) {
        if (is_game_over(board)) {
            // no available moves + game is over
            return node;
        } else {
            // no available moves + game is NOT over, so must pass turn
            // passing a turn acts as a "move" and gets its own node
            auto pass_legal_moves = legal_moves(board, opponent(this->color));
            auto board_copy = board;
            GameState pass_state{ std::move(board_copy), std::move(pass_legal_moves), opponent(this->color) };
            ReversiNode& pass_node = this->reversi_tree.add_node(std::move(pass_state), { 0, 0 }, node); // TODO: move coord should not be {0, 0}
            return pass_node;
        }
    } else {
        // this node has children to select from

        const auto& legal_moves_vec = game_state.get_legal_moves();
        if (legal_moves_vec.size() > node.get_children().size()) {
            // there are more possible child states than there are currently child nodes
            // so we must be able to create new child nodes for these states

            // find a move that does not have a node yet, and create a node for it
            const auto& child_nodes = node.get_children();
            auto exists_child_with_move = [&child_nodes](auto& move) { // TODO this should be looked up in an unordered_set
                for (const auto& child : child_nodes) {
                    if (child->get_move() == move) {
                        return true;
                    }
                }
                return false;
            };

            for (const auto& move : legal_moves_vec) {
                if (!exists_child_with_move(move)) {
                    // we have found a move that does not have a node yet, so create it
                    Board next_board{ board };
                    Piece color = game_state.get_player_turn();
                    apply_move(next_board, color, move);
                    auto next_moves = legal_moves(next_board, opponent(color));
                    GameState next_state{ std::move(next_board), std::move(next_moves), opponent(color) };
                    auto& next_node = this->reversi_tree.add_node(std::move(next_state), move, node);
                    return next_node;
                }
            }
        }

        // there was not an unplayed child, so recurse with UCB
        return this->tree_policy(best_child(node));
    }
}

void MonteCarloAgent::back_propagate(ReversiNode& node, unsigned result)
{
    ReversiNode* parent_ptr = &node;
    while (parent_ptr != nullptr) {
        parent_ptr->increment_plays();
        parent_ptr->update_wins(result);

        parent_ptr = parent_ptr->get_parent();
    }
}

unsigned MonteCarloAgent::simulate(ReversiNode& node)
{
    const unsigned WIN_RESULT = 1;
    const unsigned LOSS_RESULT = 0;

    // play a random game
    Board board_copy{ node.get_game_state().get_board() };
    auto random_agent_black = std::unique_ptr<Agent>(new random_agent{ black });
    auto random_agent_white = std::unique_ptr<Agent>(new random_agent{ white });

    Piece winner = play_game(board_copy, *random_agent_black, *random_agent_white, node.get_game_state().get_player_turn(), true); // this is where the slowdown is

    if (winner == this->color) {
        return WIN_RESULT;
    } else {
        return LOSS_RESULT;
    }
}

ReversiNode& MonteCarloAgent::best_child(ReversiNode& root)
{
    const bool is_enemy_turn = root.get_game_state().get_player_turn() != this->color;
    const double C = 1; // exploration value
    std::vector<std::pair<ReversiNode*, double>> node_scores;
    unsigned parent_plays = root.get_plays();

    for (const auto& child_ptr : root.get_children()) {
        unsigned wins = child_ptr->get_wins();
        unsigned plays = child_ptr->get_plays();
        enforce(plays > 0, "every child needs to have been played before calling this");

        if (is_enemy_turn) {
            wins = plays - wins;
        }

        double score = (wins / (double)plays) + C * std::sqrt(2 * std::log(parent_plays) / (double)plays);
        auto node_score = std::make_pair(child_ptr.get(), score);
        node_scores.push_back(node_score);
    }

    double best_score = std::numeric_limits<double>::lowest();
    ReversiNode* best_node_ptr = nullptr;
    for (const auto& child_score_pair : node_scores) {
        double score = std::get<double>(child_score_pair);
        if (score > best_score) {
            best_score = score;
            best_node_ptr = std::get<ReversiNode*>(child_score_pair);
        }
    }

    return *best_node_ptr;
}

std::string MonteCarloAgent::node_scores_str(const std::vector<std::unique_ptr<ReversiNode>>& nodes) const
{
    std::vector<std::reference_wrapper<const ReversiNode>> sorted{};
    for (const auto& node : nodes) {
        sorted.push_back(std::cref(*node));
    }

    std::sort(sorted.begin(), sorted.end(),
        [](const ReversiNode& a, const ReversiNode& b) {
            return a.get_plays() < b.get_plays();
        });

    std::stringstream stream{};

    for (const ReversiNode& node : sorted) {
        const auto wins = node.get_wins();
        const auto plays = node.get_plays();
        const auto move = node.get_move();

        const double percent_win = 100 * wins / (double)plays;

        stream << move.stringify() << ": wins/plays " << wins << "/" << plays << "(" << std::setprecision(2) << percent_win << "%)" << std::endl;
    }

    return stream.str();
}

ReversiNode& MonteCarloAgent::winningest_node(const std::vector<std::unique_ptr<ReversiNode>>& nodes) const
{
    ReversiNode* best_node = nullptr;
    unsigned most_plays = 0;
    unsigned most_wins_tiebreaker = 0;

    for (const auto& node : nodes) {
        unsigned plays = node->get_plays();
        unsigned wins = node->get_wins();
        if (plays > most_plays || (plays == most_plays && wins > most_wins_tiebreaker)) {
            most_plays = plays;
            most_wins_tiebreaker = wins;
            best_node = node.get();
        }
    }

    return *best_node;
}