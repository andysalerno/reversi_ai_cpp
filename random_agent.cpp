#include "random_agent.hpp"
#include "coord.hpp"
#include <vector>
#include "board.hpp"
#include "util.hpp"

coord random_agent::pick_move(board &_board, std::vector<coord> &legal_moves)
{
    return vec_pick_random(legal_moves);
}