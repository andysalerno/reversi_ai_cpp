#ifndef MONTE_CARLO_AGENT_H
#define MONTE_CARLO_AGENT_H

#include "agent.hpp"
#include <vector>
#include <unordered_map>
#include "coord.hpp"
#include "board.hpp"
#include "node.hpp"
#include "game_state.hpp"

class monte_carlo_agent : public agent
{
    coord mcts(board &_board, std::vector<coord> &legal_moves);
    std::unordered_map<game_state, node> state_node;

  public:
    monte_carlo_agent();
    coord pick_move(board &, std::vector<coord> &legal_moves);
};

#endif