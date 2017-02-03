#ifndef COORD_HPP
#define COORD_HPP

#include <string>

struct coord
{
    unsigned x;
    unsigned y;

    std::string stringify() const
    {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
    }
};

#endif