#ifndef GENERAL_H
#define GENERAL_H

#include <chrono>
#include <random>

// Function of comparison
template<typename T>
bool smallerToHigher(const T& param1, const T& param2);

template<typename T>
bool smallerToHigher(const T &param1, const T &param2)
{
    return param1 < param2;
}

// Random number generator
static std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());

#endif // GENERAL_H
