#include "random_agent.hpp"
#include "board.hpp"
#include "coord.hpp"
#include "util.hpp"
#include <vector>

coord random_agent::pick_move(const GameState& game_state)
{
    return vec_pick_random(game_state.get_legal_moves());
}