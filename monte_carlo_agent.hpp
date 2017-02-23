#ifndef MONTE_CARLO_AGENT_H
#define MONTE_CARLO_AGENT_H

#include "agent.hpp"
#include "board.hpp"
#include "coord.hpp"

class MonteCarloAgent : public agent {
public:
    coord pick_move(const Board&, std::vector<coord>& legal_moves);
    MonteCarloAgent(Piece _color)
        : agent(_color)
    {
    }
};

#endif