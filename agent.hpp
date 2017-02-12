#ifndef AGENT_H
#define AGENT_H

#include "board.hpp"
#include "coord.hpp"
#include <vector>

class agent {
public:
    Piece color;
    virtual ~agent() {}
    virtual coord pick_move(const Board&, std::vector<coord>& legal_moves) = 0;

    agent(Piece _color)
        : color(_color)
    {
    }
};

#endif