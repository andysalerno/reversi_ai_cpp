#ifndef COORD_HPP
#define COORD_HPP

#include <string>

struct Coord {
    unsigned x = 0;
    unsigned y = 0;

    std::string stringify() const
    {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
    }
};

inline bool operator==(const Coord& lhs, const Coord& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

namespace std {
template <>
struct hash<Coord> {
    std::size_t operator()(Coord const& coord) const
    {
        return (coord.x * 127) + coord.y;
    }
};
}

struct direction {
    int dx;
    int dy;
};

#endif