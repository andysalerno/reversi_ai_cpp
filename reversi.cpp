#include "reversi.hpp"
#include "board.hpp"
#include "coord.hpp"
#include <vector>
#include <iostream>
#include "util.hpp"
#include <cassert>
#include "agent.hpp"
#include "random_agent.hpp"
#include <memory>

int main()
{
    board _board{8};

    auto black_agent = std::unique_ptr<agent>(new random_agent{});
    auto white_agent = std::unique_ptr<agent>(new random_agent{});

    play_game(_board, black_agent, white_agent);
}

void play_game(board &_board, std::unique_ptr<agent> &black_agent, std::unique_ptr<agent> &white_agent)
{
    initialize_reversi_board(_board);

    std::cout << _board.stringify();
    while (!is_game_over(_board))
    {
        // black makes a move (if it can)
        auto legal_black = legal_moves(_board, black);
        if (legal_black.size() > 0)
        {
            coord move = black_agent->pick_move(_board, legal_black);
            std::cout << move.stringify() << '\n';
            apply_move(_board, move, black);
        }
        std::cout << _board.stringify();

        // white makes a move (if it can)
        auto legal_white = legal_moves(_board, white);
        if (legal_white.size() > 0)
        {
            coord move = white_agent->pick_move(_board, legal_white);
            std::cout << move.stringify() << '\n';
            apply_move(_board, move, white);
        }
        std::cout << _board.stringify();
    }

    std::cout << "Game over!!\n";
}

std::vector<coord> legal_moves(board &_board, Piece player_color)
{
    std::vector<coord> ret;
    for (unsigned y = 0; y < _board.get_size(); ++y)
    {
        for (unsigned x = 0; x < _board.get_size(); ++x)
        {
            coord xy = {x, y};
            if (is_legal_move(xy, _board, player_color))
            {
                ret.push_back(xy);
            }
        }
    }
    return ret;
}

bool is_game_over(board &_board)
{
    if (_board.is_full())
    {
        return true;
    }
    else if (legal_moves(_board, white).size() == 0 && legal_moves(_board, black).size() == 0)
    {
        return true;
    }
    return false;
}

bool is_legal_move(coord &move, board &_board, Piece player_color)
{
    if (!_board.is_in_bounds(move) || _board.get_piece(move) != empty)
    {
        return false;
    }

    for (int dy = -1; dy < 2; ++dy)
    {
        for (int dx = -1; dx < 2; ++dx)
        {
            if (dx == 0 && dy == 0)
            {
                continue;
            }

            if (is_direction_valid_move(_board, move, player_color, dx, dy))
            {
                return true;
            }
        }
    }

    return false;
}

bool is_direction_valid_move(board &_board, coord &move, Piece player_color, int dx, int dy)
{
    if (dx == 0 && dy == 0)
    {
        return false;
    }

    unsigned dist = 0;
    while (true)
    {
        dist += 1;
        unsigned xp = move.x + (dist * dx);
        unsigned yp = move.y + (dist * dy);
        if (!_board.is_in_bounds({xp, yp}))
        {
            return false;
        }

        Piece check_piece = _board.get_piece({xp, yp});
        if (dist == 1 && check_piece != opponent(player_color))
        {
            // the first closest piece MUST be an opponent, not
            // empty or player color. If this is not the case
            // then this is not a valid capture direction.
            return false;
        }
        else if (dist > 1 && check_piece == empty)
        {
            // If we hit an empty piece before hitting our own color,
            // this is not a valid direction to flip.
            return false;
        }
        else if (dist > 1 && check_piece == opponent(player_color))
        {
            // If we hit an opponent piece, keep checking in this direction
            continue;
        }
        else if (dist > 1 && check_piece == player_color)
        {
            // If we hit our piece, we know there is some amount of opponent
            // pieces between our new piece and our existing piece, with no empty
            // spaces in between, making this direction a valid flip direction.
            return true;
        }
    }
}

bool apply_move(board &_board, coord &move, Piece player_color)
{
    std::vector<direction> directions_to_flip;

    // find directions that need to be flipped
    for (int dy = -1; dy < 2; ++dy)
    {
        for (int dx = -1; dx < 2; ++dx)
        {
            if (is_direction_valid_move(_board, move, player_color, dx, dy))
            {
                directions_to_flip.push_back({dx, dy});
            }
        }
    }

    if (directions_to_flip.size() == 0)
    {
        return false;
    }

    // perform the flipping
    for (const auto &direction : directions_to_flip)
    {
        unsigned distance = 1;
        while (true)
        {
            coord flip_coord = {move.x + (direction.dx * distance), move.y + (direction.dy * distance)};
            Piece flip_piece = _board.get_piece(flip_coord);
            assert(flip_piece == player_color || flip_piece == opponent(player_color));
            if (flip_piece == opponent(player_color))
            {
                _board.flip_piece(flip_coord);
            }
            else if (flip_piece == player_color)
            {
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
    if (player == white)
    {
        return black;
    }
    else if (player == black)
    {
        return white;
    }
}

void initialize_reversi_board(board &_board)
{
    _board.clear();

    unsigned midpoint_lower = (_board.get_size() - 1) / 2;
    unsigned midpoint_upper = midpoint_lower + 1;

    coord top_left = {midpoint_lower, midpoint_upper};
    coord top_right = {midpoint_upper, midpoint_upper};
    coord bot_left = {midpoint_lower, midpoint_lower};
    coord bot_right = {midpoint_upper, midpoint_lower};

    _board.set_piece(top_left, black);
    _board.set_piece(top_right, white);
    _board.set_piece(bot_left, white);
    _board.set_piece(bot_right, black);
}