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
bool less(const T& param1, const T& param2);

template<typename T>
std::basic_string<T> lowerCase(std::basic_string<T>& s);

template<typename T>
T getNumber(std::string& s);

template<typename T>
bool crowdingOperator(const T& param1, const T& param2);

template<typename T>
class Ascending
{
public:
    int index;
    bool operator()(const T& param1, const T& param2) const
    {
        return param1.getFitness()[index] < param2.getFitness()[index];
    }
};

template<typename T>
bool emptyPopulation(const T& param);

// Definitions

template<typename T>
bool less(const T &param1, const T &param2)
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

template<typename T>
bool crowdingOperator(const T& param1, const T& param2)
{
    return (param1.getRank() < param2.getRank())
            || ((param1.getRank() == param2.getRank()) && (param1.getDistance() > param2.getDistance()));
}

template<typename T>
bool emptyPopulation(const T& param)
{
    return param.getCurrentNbChromosomes() != 0;
}

// Random number generator
static std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());

#endif // GENERAL_H
