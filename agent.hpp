#ifndef AGENT_H
#define AGENT_H

#include "board.hpp"
#include "coord.hpp"
#include <vector>

class agent {
public:
    virtual ~agent() {}
    virtual coord pick_move(Board&, std::vector<coord>& legal_moves) = 0;
};

#endif