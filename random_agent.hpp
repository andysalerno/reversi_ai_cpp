#ifndef RANDOM_AGENT_H
#define RANDOM_AGENT_H

#include "agent.hpp"
#include "board.hpp"
#include "coord.hpp"

class random_agent : public Agent {
public:
    Coord pick_move(const GameState&);
    random_agent(Piece color)
        : Agent(color)
    {
    }
};

#endif