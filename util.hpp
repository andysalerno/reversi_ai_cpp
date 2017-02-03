#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <random>

template<typename T>
T vec_pick_random(const std::vector<T> &vec)
{
    std::random_device rand;
    std::mt19937 engine{rand()};
    std::uniform_int_distribution<int> index(0, vec.size() - 1);
    return vec[index(engine)];
}

#endif