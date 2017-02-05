#ifndef AGENT_H
#define AGENT_H

#include <vector>
#include "coord.hpp"
#include "board.hpp"

class agent
{
  public:
    virtual ~agent() {}
    virtual coord pick_move(board &, std::vector<coord> &legal_moves) = 0;
};

#endif