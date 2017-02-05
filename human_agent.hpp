#ifndef HUMAN_AGENT_H
#define HUMAN_AGENT_H

#include "agent.hpp"
#include <vector>
#include "board.hpp"

class human_agent : public agent
{
  public:
    coord pick_move(board &, std::vector<coord> &legal_moves);
};

#endif