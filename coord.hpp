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

namespace std {
template <>
struct hash<coord> {
    std::size_t operator()(coord const& _coord) const
    {
        return (_coord.x * 127) + _coord.y;
    }
};
}

struct direction {
    int dx;
    int dy;
};

#endif