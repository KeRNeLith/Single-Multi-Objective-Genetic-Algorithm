#ifndef GENERAL_H
#define GENERAL_H

#include <chrono>
#include <random>
#include <locale>
#include <string>
#include <sstream>
#include <algorithm>
#include <limits>

/////////////////////////////////////////////////////////////
//////////////////////// Declarations ///////////////////////
/////////////////////////////////////////////////////////////

template<typename T>
/**
 * @brief less Compare the 2 parameters.
 * @param param1 First Operand.
 * @param param2 Second Operand.
 * @return true if param1 < param2, otherwise return false.
 */
bool less(const T& param1, const T& param2);

template<typename T>
/**
 * @brief lowerCase Convert a string in lower case.
 * @param s String to convert.
 * @return String in lower case.
 */
std::basic_string<T> lowerCase(std::basic_string<T>& s);

template<typename T>
/**
 * @brief getNumber Extract a number of type T in a string.
 * @param s String in which we want to extract a number.
 * @return Number extrated with the type T.
 */
T getNumber(std::string& s);

template<typename T>
/**
 * @brief crowdingOperator Compare the 2 parameters according to their crowding.
 * @param param1 First Operand.
 * @param param2 Second Operand.
 * @return true if (rank param1 < rank param2) or (rank param1 = rank param2 and distance param1 > distance param2), otherwise return false.
 */
bool crowdingOperator(const T& param1, const T& param2);

template<typename T>
/**
 * @brief The Ascending class Use to perform comparisons pair by pair in ascending order.
 */
class Ascending
{
public:
    int index;
    virtual bool operator()(const T& param1, const T& param2) const
    {
        return param1.getFitness()[index] < param2.getFitness()[index];
    }
};

template<typename T>
/**
 * @brief emptyPopulation Check if the popualtion is empty.
 * @param param Population to check.
 * @return true if it's empty, otherwise return false.
 */
bool emptyPopulation(const T& param);

template <typename T>
int getDecimalFromBinary(std::vector<T> binary);

template <typename T>
T power(T nb, int pow);

// Random number generator
static std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());


/////////////////////////////////////////////////////////////
//////////////////////// Definitions ////////////////////////
/////////////////////////////////////////////////////////////

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
    return param.getCurrentNbChromosomes() == 0;
}

template <typename T>
int getDecimalFromBinary(std::vector<T> binary)
{
    int value = 0;
    int index = 0;
    for (unsigned int i = binary.size()-1 ; i >= 1 ; i--)
    {
        if (binary[i] == 1)
            value += power(2, index);
        index++;
    }

    if (binary[0] == 1)
        value = -value;

    return value;
}

template <typename T>
T power(T nb, int pow)
{
    if (pow < 0)
        return -1;

    if (pow == 0)
        return 1;

    T result = 1;
    for (int i = 1 ; i <= pow ; i++)
        result *= nb;

    return result;
}

#endif // GENERAL_H
