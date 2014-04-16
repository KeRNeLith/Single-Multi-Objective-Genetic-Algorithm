#ifndef GENERAL_H
#define GENERAL_H

#include <chrono>
#include <random>
#include <locale>
#include <string>
#include <sstream>
#include <algorithm>
#include <limits>

// Declarations

template<typename T>
bool smallerToHigher(const T& param1, const T& param2);

template<typename T>
std::basic_string<T> lowerCase(std::basic_string<T>& s);

template<typename T>
T getNumber(std::string& s);


// Definitions

template<typename T>
bool smallerToHigher(const T &param1, const T &param2)
{
    return param1 < param2;
}

template<typename T>
std::basic_string<T> lowerCase(std::basic_string<T>& s)
{
    std::transform(s.begin(), s.end(), s.begin(), tolower);
    return s;
}

template<typename T>
T getNumber(std::string& s)
{
    std::stringstream ss(s);
    T ret;
    return ss >> ret ? ret : std::numeric_limits<T>::max();
}

// Random number generator
static std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());

#endif // GENERAL_H
