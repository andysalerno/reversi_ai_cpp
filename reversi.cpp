#include "reversi.hpp"
#include "agent.hpp"
#include "board.hpp"
#include "coord.hpp"
#include "gamestate.hpp"
#include "human_agent.hpp"
#include "random_agent.hpp"
#include "util.hpp"
#include <cassert>
#include <iostream>
#include <memory>
#include <vector>

int main()
{
    Board _board;

    auto black_agent = std::unique_ptr<agent>(new random_agent{ black });
    auto white_agent = std::unique_ptr<agent>(new human_agent{ white });

    initialize_reversi_board(_board);
    Piece winner = play_game(_board, black_agent, white_agent, black);
}

Piece play_game(Board& _board, std::unique_ptr<agent>& black_agent, std::unique_ptr<agent>& white_agent, Piece player_turn, bool silent /* default false */)
{
    enforce(player_turn == white || player_turn == black, "player_turn must be white or black");
    const std::string black_name{ "Black" };
    const std::string white_name{ "White" };

    show(_board.stringify(), silent);

    agent* whose_turn;
    const std::string* color_name;
    if (player_turn == white) {
        whose_turn = white_agent.get();
        color_name = &white_name;
    } else if (player_turn == black) {
        whose_turn = black_agent.get();
        color_name = &black_name;
    }

    while (!is_game_over(_board)) {
        auto moves = legal_moves(_board, whose_turn->color);
        GameState game_state{ _board, moves, whose_turn->color };
        if (moves.size() > 0) {
            coord move = whose_turn->pick_move(game_state);
            show(*color_name + " plays at: " + move.stringify() + '\n', silent);
            apply_move(_board, whose_turn->color, move);
        } else {
            show(*color_name + " has no moves, passes turn.\n", silent);
        }
        show(_board.stringify(), silent);

        if (whose_turn == white_agent.get()) {
            whose_turn = black_agent.get();
            color_name = &black_name;
        } else {
            whose_turn = white_agent.get();
            color_name = &white_name;
        }
    }

    show("Game over!!\n", silent);

    return most_pieces(_board);
}

std::vector<coord> legal_moves(const Board& _board, Piece player_color)
{
    // TODO: implement cache
    std::vector<coord> ret;
    for (unsigned y = 0; y < _board.get_size(); ++y) {
        for (unsigned x = 0; x < _board.get_size(); ++x) {
            coord xy = { x, y };
            if (is_legal_move(xy, _board, player_color)) {
                ret.push_back(xy);
            }
        }
    }
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

bool is_legal_move(coord& move, const Board& _board, Piece player_color)
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

bool is_direction_valid_move(const Board& _board, const coord& move, Piece player_color, int dx, int dy)
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

bool apply_move(Board& _board, Piece player_color, const coord& move)
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
            coord flip_coord = { move.x + (direction.dx * distance), move.y + (direction.dy * distance) };
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
    if (player == white) {
        return black;
    } else if (player == black) {
        return white;
    }
}

void initialize_reversi_board(Board& _board)
{
    _board.clear();

    unsigned midpoint_lower = (_board.get_size() - 1) / 2;
    unsigned midpoint_upper = midpoint_lower + 1;

    coord top_left = { midpoint_lower, midpoint_upper };
    coord top_right = { midpoint_upper, midpoint_upper };
    coord bot_left = { midpoint_lower, midpoint_lower };
    coord bot_right = { midpoint_upper, midpoint_lower };

    _board.set_piece(top_left, black);
    _board.set_piece(top_right, white);
    _board.set_piece(bot_left, white);
    _board.set_piece(bot_right, black);
}