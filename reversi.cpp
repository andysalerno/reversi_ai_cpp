#include "reversi.hpp"
#include "board.hpp"
#include "coord.hpp"
#include <vector>
#include <iostream>
#include "util.hpp"

int main()
{
    board _board{8};
    play_game(_board);
}

void play_game(board &_board)
{
    initialize_reversi_board(_board);
    std::cout << _board.stringify();

    while (!is_game_over(_board))
    {
        auto legal_black = legal_moves(_board, black);
        if (legal_black.size() > 0)
        {
            auto move = vec_pick_random(legal_black);
            std::cout << move.stringify() << '\n';
            return;
        }
    }
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

    for (int yd = -1; yd < 2; ++yd)
    {
        for (int xd = -1; xd < 2; ++xd)
        {
            if (xd == 0 && yd == 0)
            {
                continue;
            }

            unsigned dist = 0;
            while (true)
            {
                dist += 1;
                unsigned xp = move.x + (dist * xd);
                unsigned yp = move.y + (dist * yd);
                if (!_board.is_in_bounds({xp, yp}))
                {
                    break;
                }

                Piece check_piece = _board.get_piece({xp, yp});
                if (dist == 1 && check_piece != opponent(player_color))
                {
                    // the first closest piece MUST be an opponent, not
                    // empty or player color. If this is not the case
                    // then this is not a valid capture direction.
                    break;
                }
                else if (dist > 1 && check_piece == opponent(player_color))
                {
                    continue;
                }
                else if (dist > 1 && check_piece == player_color)
                {
                    // we have found a direction where there is some
                    // amount of opponent pieces, followed by a piece of the player's color.
                    return true;
                }
            }
        }
    }

    return false;
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