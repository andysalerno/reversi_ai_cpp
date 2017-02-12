#ifndef COORD_HPP
#define COORD_HPP

#include <string>

struct coord {
    unsigned x;
    unsigned y;

    std::string stringify() const
    {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
    }
};

inline bool operator==(const coord& lhs, const coord& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

struct direction {
    int dx;
    int dy;
};

#endif