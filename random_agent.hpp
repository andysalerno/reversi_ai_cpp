#ifndef RANDOM_AGENT_H
#define RANDOM_AGENT_H

#include "agent.hpp"
#include "board.hpp"
#include "coord.hpp"

class random_agent : public agent {
public:
    coord pick_move(const Board&, std::vector<coord>& legal_moves);
    random_agent(Piece color)
        : agent(color)
    {
    }
};

#endif