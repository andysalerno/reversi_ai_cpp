#ifndef HUMAN_AGENT_H
#define HUMAN_AGENT_H

#include "agent.hpp"
#include "board.hpp"
#include <vector>

class human_agent : public Agent {
public:
    Coord pick_move(const GameState&);
    human_agent(Piece color)
        : Agent(color)
    {
    }
};

#endif