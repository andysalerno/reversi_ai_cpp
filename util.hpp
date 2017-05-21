#ifndef UTIL_H
#define UTIL_H

#include <cassert>
#include <iostream>
#include <random>
#include <string>
#include <vector>

static std::mt19937 rand_gen{ std::random_device{}() };

template <typename T>
T vec_pick_random(const std::vector<T>& vec)
{
    std::uniform_int_distribution<int> index{ 0, static_cast<int>(vec.size()) - 1 };
    return vec[index(rand_gen)];
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