#ifndef AGENT_H
#define AGENT_H

#include "board.hpp"
#include "coord.hpp"
#include "gamestate.hpp"
#include <vector>

class agent {
public:
    Piece color;
    virtual ~agent() {}
    virtual coord pick_move(const GameState&) = 0;

    agent(Piece _color)
        : color(_color)
    {
    }
};

#endif