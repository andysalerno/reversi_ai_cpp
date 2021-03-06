#include "board.hpp"
#include <cmath>
#include <iostream>
#include <utility>

Board::Board()
{
    this->clear();
}

void Board::flip_piece(Coord xy)
{
    if (this->get_piece(xy) == black) {
        this->amount_black_pieces -= 1;
        this->set_piece(xy, white);
    } else if (this->get_piece(xy) == white) {
        this->amount_white_pieces -= 1;
        this->set_piece(xy, black);
    }
}

bool Board::is_full() const
{
    return (this->get_amount_black() + this->get_amount_white() == pow(this->get_size(), 2));
}

unsigned Board::get_amount_black() const
{
    return this->amount_black_pieces;
}

unsigned Board::get_amount_white() const
{
    return this->amount_white_pieces;
}

void Board::clear()
{
    this->amount_black_pieces = this->amount_white_pieces = 0;
    std::fill(this->board_vec.begin(), this->board_vec.end(),
        std::vector<Piece>(this->size, empty));
}

size_t Board::get_size() const
{
    return this->size;
}

void Board::set_piece(Coord xy, Piece piece)
{
    this->board_vec[xy.y][xy.x] = piece;

    if (piece == white) {
        this->amount_white_pieces += 1;
    } else if (piece == black) {
        this->amount_black_pieces += 1;
    }
}

Piece Board::get_piece(Coord xy) const
{
    return this->board_vec[xy.y][xy.x];
}

bool Board::is_in_bounds(Coord xy) const
{
    if (xy.x < 0 || xy.y < 0) {
        return false;
    }
    if (xy.x >= this->size || xy.y >= this->size) {
        return false;
    }
    return true;
}

std::string Board::stringify()
{
    std::string s = "";

    unsigned row_count = 0;
    for (const auto& row : this->board_vec) {
        std::string line = "";
        for (const auto& row_val : row) {
            if (row_val == empty) {
                line += " ";
            } else if (row_val == black) {
                line += "X";
            } else if (row_val == white) {
                line += "O";
            }

            line += " ";
        }
        s = std::to_string(row_count) + " " + line + '\n' + s;
        ++row_count;
    }

    std::string bottom_nums = "  ";
    for (unsigned x = 0; x < this->get_size(); ++x) {
        bottom_nums += std::to_string(x) + " ";
    }
    bottom_nums += '\n';
    s += bottom_nums;
    return s;
}