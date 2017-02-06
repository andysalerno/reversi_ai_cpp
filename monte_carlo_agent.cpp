#include <vector>
#include "board.hpp"
#include "coord.hpp"
#include "monte_carlo_agent.hpp"

monte_carlo_agent::monte_carlo_agent()
    //: state_node(std::unordered_map<game_state, node>{})
{
}

coord monte_carlo_agent::pick_move(board &_board, std::vector<coord> &legal_moves)
{
    return this->mcts(_board, legal_moves);
}

coord monte_carlo_agent::mcts(board &_board, std::vector<coord> &legal_moves)
{
}