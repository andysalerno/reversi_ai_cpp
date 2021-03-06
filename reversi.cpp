#include "reversi.hpp"
#include "agent.hpp"
#include "board.hpp"
#include "cache_dict.hpp"
#include "coord.hpp"
#include "gamestate.hpp"
#include "human_agent.hpp"
#include "monte_carlo_agent.hpp"
#include "random_agent.hpp"
#include "util.hpp"
#include <cassert>
#include <functional>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

static CacheDict<Board, std::vector<Coord>> white_cache{};
static CacheDict<Board, std::vector<Coord>> black_cache{};

int main()
{
    Board _board;

    auto black_agent = std::unique_ptr<Agent>(new MonteCarloAgent{ black });
    auto white_agent = std::unique_ptr<Agent>(new MonteCarloAgent{ white });

    initialize_reversi_board(_board);
    Piece winner = play_game(_board, *black_agent, *white_agent, black);

    std::string black_name("black");
    std::string white_name("white");
    std::string* winner_ptr;
    if (winner == white)
        winner_ptr = &white_name;
    else
        winner_ptr = &black_name;
    std::cout << "winner: " << *winner_ptr << std::endl;
}

Piece play_game(Board& _board, Agent& black_agent, Agent& white_agent, Piece player_turn, bool silent /* default false */)
{
    enforce(player_turn == white || player_turn == black, "player_turn must be white or black");
    const std::string black_name{ "Black" };
    const std::string white_name{ "White" };

    show(_board.stringify(), silent);

    Agent* whose_turn;
    const std::string* color_name;
    if (player_turn == white) {
        whose_turn = &white_agent;
        color_name = &white_name;
    } else if (player_turn == black) {
        whose_turn = &black_agent;
        color_name = &black_name;
    } else {
        enforce(false, "player_turn was not white or black");
        color_name = nullptr;
        whose_turn = nullptr;
    }

    while (!is_game_over(_board)) {
        auto moves = legal_moves(_board, whose_turn->color);
        GameState game_state{ _board, moves, whose_turn->color };
        if (moves.size() > 0) {
            Coord move = whose_turn->pick_move(game_state);
            show(*color_name + " plays at: " + move.stringify() + '\n', silent);
            apply_move(_board, whose_turn->color, move);
        } else {
            show(*color_name + " has no moves, passes turn.\n", silent);
        }
        show(_board.stringify(), silent);

        if (whose_turn == &white_agent) {
            whose_turn = &black_agent;
            color_name = &black_name;
        } else {
            whose_turn = &white_agent;
            color_name = &white_name;
        }
    }

    show("Game over!!\n", silent);

    return most_pieces(_board);
}

std::vector<Coord> legal_moves(const Board& _board, Piece player_color)
{
    std::vector<Coord>* cached = player_color == white ? white_cache.get(_board) : black_cache.get(_board);
    if (cached != nullptr) {
        return *cached;
    }

    std::vector<Coord> ret;
    for (unsigned y = 0; y < _board.get_size(); ++y) {
        for (unsigned x = 0; x < _board.get_size(); ++x) {
            Coord xy = { x, y };
            if (is_legal_move(xy, _board, player_color)) {
                ret.push_back(xy);
            }
        }
    }

    player_color == white ? white_cache.save(_board, ret) : black_cache.save(_board, ret);

    return ret;
}

bool is_game_over(const Board& _board)
{
    if (_board.is_full()) {
        return true;
    } else if (legal_moves(_board, white).size() == 0 && legal_moves(_board, black).size() == 0) {
        return true;
    }
    return false;
}

Piece most_pieces(const Board& board)
{
    if (board.get_amount_white() == board.get_amount_black()) {
        return empty; // tie returns neutral empty "color"
    } else if (board.get_amount_black() > board.get_amount_white()) {
        return black;
    } else {
        return white;
    }
}

bool is_legal_move(Coord& move, const Board& _board, Piece player_color)
{
    if (!_board.is_in_bounds(move) || _board.get_piece(move) != empty) {
        return false;
    }

    for (int dy = -1; dy < 2; ++dy) {
        for (int dx = -1; dx < 2; ++dx) {
            if (dx == 0 && dy == 0) {
                continue;
            }

            if (is_direction_valid_move(_board, move, player_color, dx, dy)) {
                return true;
            }
        }
    }

    return false;
}

bool is_direction_valid_move(const Board& _board, const Coord& move, Piece player_color, int dx, int dy)
{
    if (dx == 0 && dy == 0) {
        return false;
    }

    unsigned dist = 0;
    while (true) {
        dist += 1;
        unsigned xp = move.x + (dist * dx);
        unsigned yp = move.y + (dist * dy);
        if (!_board.is_in_bounds({ xp, yp })) {
            return false;
        }

        Piece check_piece = _board.get_piece({ xp, yp });
        if (dist == 1 && check_piece != opponent(player_color)) {
            // the first closest piece MUST be an opponent, not
            // empty or player color. If this is not the case
            // then this is not a valid capture direction.
            return false;
        } else if (dist > 1 && check_piece == empty) {
            // If we hit an empty piece before hitting our own color,
            // this is not a valid direction to flip.
            return false;
        } else if (dist > 1 && check_piece == opponent(player_color)) {
            // If we hit an opponent piece, keep checking in this direction
            continue;
        } else if (dist > 1 && check_piece == player_color) {
            // If we hit our piece, we know there is some amount of opponent
            // pieces between our new piece and our existing piece, with no empty
            // spaces in between, making this direction a valid flip direction.
            return true;
        }
    }
}

bool apply_move(Board& _board, Piece player_color, const Coord& move)
{
    std::vector<direction> directions_to_flip;

    // find directions that need to be flipped
    for (int dy = -1; dy < 2; ++dy) {
        for (int dx = -1; dx < 2; ++dx) {
            if (is_direction_valid_move(_board, move, player_color, dx, dy)) {
                directions_to_flip.push_back({ dx, dy });
            }
        }
    }

    if (directions_to_flip.size() == 0) {
        return false;
    }

    // perform the flipping
    for (const auto& direction : directions_to_flip) {
        unsigned distance = 1;
        while (true) {
            Coord flip_coord = { move.x + (direction.dx * distance), move.y + (direction.dy * distance) };
            Piece flip_piece = _board.get_piece(flip_coord);
            assert(flip_piece == player_color || flip_piece == opponent(player_color));
            if (flip_piece == opponent(player_color)) {
                _board.flip_piece(flip_coord);
            } else if (flip_piece == player_color) {
                break;
            }

            ++distance;
        }
    }

    _board.set_piece(move, player_color);
    return true;
}

Piece opponent(Piece player)
{
    enforce(player == white || player == black, "opponent requires white or black");
    if (player == white) {
        return black;
    } else if (player == black) {
        return white;
    }
    return empty;
}

void initialize_reversi_board(Board& _board)
{
    _board.clear();

    unsigned midpoint_lower = (_board.get_size() - 1) / 2;
    unsigned midpoint_upper = midpoint_lower + 1;

    Coord top_left = { midpoint_lower, midpoint_upper };
    Coord top_right = { midpoint_upper, midpoint_upper };
    Coord bot_left = { midpoint_lower, midpoint_lower };
    Coord bot_right = { midpoint_upper, midpoint_lower };

    _board.set_piece(top_left, black);
    _board.set_piece(top_right, white);
    _board.set_piece(bot_left, white);
    _board.set_piece(bot_right, black);
}