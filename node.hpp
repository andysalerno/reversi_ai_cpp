#ifndef NODE_H
#define NODE_H

#include <vector>
#include "game_state.hpp"

class node
{
    unsigned plays;
    unsigned wins;
    node &parent;
    std::vector<node> children;
    game_state &game_state;
};

#endif