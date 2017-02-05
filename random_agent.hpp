#ifndef RANDOM_AGENT_H
#define RANDOM_AGENT_H

#include "agent.hpp"
#include "coord.hpp"
#include "board.hpp"

class random_agent : public agent
{
  public:
    coord pick_move(board &, std::vector<coord> &legal_moves);
};

#endif