#include "board.hpp"

board::board(size_t size) : size(size), board_vec(size, std::vector<Piece>(size, black)), amount_black_pieces(0), amount_white_pieces(0)
{
    this->clear();
}

void board::flip_piece(coord xy)
{
    if (this->get_piece(xy) == black)
    {
        --(this->amount_black_pieces);
        this->set_piece(xy, white);
    }
    else if (this->get_piece(xy) == white)
    {
        --(this->amount_white_pieces);
        this->set_piece(xy, black);
    }
}

bool board::is_full()
{
    return (this->get_amount_black() + this->get_amount_white() == this->get_size());
}

unsigned board::get_amount_black()
{
    return this->amount_black_pieces;
}

unsigned board::get_amount_white()
{
    return this->amount_white_pieces;
}

void board::clear()
{
    std::fill(this->board_vec.begin(), this->board_vec.end(), std::vector<Piece>(this->size, empty));
}

size_t board::get_size()
{
    return this->size;
}

void board::set_piece(coord xy, Piece piece)
{
    this->board_vec[xy.y][xy.x] = piece;

    if (piece == white)
    {
        ++(this->amount_white_pieces);
    }
    else if (piece == black)
    {
        ++(this->amount_black_pieces);
    }
}

Piece board::get_piece(coord xy)
{
    return this->board_vec[xy.y][xy.x];
}

bool board::is_in_bounds(coord xy)
{
    if (xy.x < 0 || xy.y < 0)
    {
        return false;
    }
    if (xy.x >= this->size || xy.y >= this->size)
    {
        return false;
    }
    return true;
}

std::string board::stringify()
{
    // TODO: stringstream
    std::string s = "";

    unsigned row_count = 0;
    for (const auto &row : this->board_vec)
    {
        std::string line = "";
        for (const auto &row_val : row)
        {
            if (row_val == empty)
            {
                line += " ";
            }
            else if (row_val == black)
            {
                line += "X";
            }
            else if (row_val == white)
            {
                line += "O";
            }

            line += " ";
        }
        s = std::to_string(row_count) + " " + line + '\n' + s;
        ++row_count;
    }

    std::string bottom_nums = "  ";
    for (unsigned x = 0; x < this->get_size(); ++x)
    {
        bottom_nums += std::to_string(x) + " ";
    }
    bottom_nums += '\n';
    s += bottom_nums;
    return s;
}