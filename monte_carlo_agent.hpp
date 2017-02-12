#ifndef MONTE_CARLO_AGENT_H
#define MONTE_CARLO_AGENT_H

#include "agent.hpp"
#include "board.hpp"
#include "coord.hpp"
#include <memory>
#include <vector>

class monte_carlo_agent : public agent {
    coord mcts(Board& _board, std::vector<coord>& legal_moves);

public:
    monte_carlo_agent();
    coord pick_move(Board&, std::vector<coord>& legal_moves);
};

#endif