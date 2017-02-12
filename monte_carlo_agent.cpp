#include "monte_carlo_agent.hpp"
#include "board.hpp"
#include "coord.hpp"
#include <vector>

monte_carlo_agent::monte_carlo_agent()
{
}

coord monte_carlo_agent::pick_move(Board& _board, std::vector<coord>& legal_moves)
{
    return this->mcts(_board, legal_moves);
}

coord monte_carlo_agent::mcts(Board& _board, std::vector<coord>& legal_moves)
{
}