#include "random_agent.hpp"
#include "board.hpp"
#include "coord.hpp"
#include "util.hpp"
#include <vector>

coord random_agent::pick_move(Board& _board, std::vector<coord>& legal_moves)
{
    return vec_pick_random(legal_moves);
}