#ifndef AGENT_H
#define AGENT_H

#include "board.hpp"
#include "coord.hpp"
#include "gamestate.hpp"
#include <vector>

class Agent {
public:
    Piece color;
    virtual ~Agent() {}
    virtual Coord pick_move(const GameState&) = 0;

    Agent(Piece _color)
        : color(_color)
    {
    }
};

#endif