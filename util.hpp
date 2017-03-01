#ifndef UTIL_H
#define UTIL_H

#include <boost/random.hpp>
#include <boost/random/random_device.hpp>
#include <cassert>
#include <iostream>
#include <random>
#include <string>
#include <vector>

template <typename T>
T vec_pick_random(const std::vector<T>& vec)
{
    boost::random_device rand;
    boost::mt19937 engine{ rand() };
    std::uniform_int_distribution<int> index(0, vec.size() - 1);
    return vec[index(engine)];
}

inline void enforce(bool expression, std::string reason)
{
    if (!expression) {
        std::cout << reason << std::endl;
        assert(false);
    }
}

inline void show(std::string s, bool silent = false)
{
    if (!silent) {
        std::cout << s;
    }
}

#endif