#ifndef GENERAL_H
#define GENERAL_H

#include <iostream>
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
/**
 * @brief getIntegerFromBinary Compute the integer decimal number corresponding to the parameter.
 * @param binary Vector of bits.
 * @return Integer decimal number.
 */
int getIntegerFromBinary(std::vector<T> binary);

template <typename T>
/**
 * @brief getUnsignedIntegerFromBinary Compute the unsigned integer decimal number corresponding to the parameter.
 * @param binary Vector of bits.
 * @return Unsigned integer decimal number.
 */
unsigned int getUnsignedIntegerFromBinary(std::vector<T> binary);

template <typename T>
/**
 * @brief getDoubleFromBinary Compute the double decimal number corresponding to the parameter.
 * @param binary Vector of bits (first 0.75% of bits correspond to the entire part of the number).
 * @return Double decimal number.
 */
double getDoubleFromBinary(std::vector<T> binary);

/**
 * @brief power Compute the number nb^pow.
 * @param nb Number we want the power.
 * @param pow Power wanted.
 * @return nb^pow.
 */
template <typename T>
T power(T nb, int pow);

union number
{
    double d;
    uint64_t i;
};

/**
 * @brief display Display bits composing the double pass in parameter.
 * @param d Double number we want bits
 */
void display(double d);
void display(union number n);

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
int getIntegerFromBinary(std::vector<T> binary)
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
unsigned int getUnsignedIntegerFromBinary(std::vector<T> binary)
{
    unsigned int value = 0;
    int index = 0;
    for (int i = binary.size()-1 ; i >= 0 ; i--)
    {
        if (binary[i] == 1)
            value += power(2, index);
        index++;
    }

    return value;
}

template <typename T>
double getDoubleFromBinary(std::vector<T> binary)
{
    /*char base_bin[binary.size()];

    for (unsigned int i = 0 ; i < binary.size() ; i++)
        base_bin[i] = binary[i] + '0';

    //char base_bin[]  = "0011111110010100011110101110000101000111101011100001010001111011";
    //char base_bin[]  = "0011111111110000000000000000000000000000000000000000000000000000";

    unsigned long int x;
    unsigned long long int value = 0;
    for (x = 0 ; x < sizeof(base_bin)-1 ; ++x)
    {
        value<<=1;
        value|=base_bin[x]&1;
    }

    return *(double *)&value;*/

    double value = 0;

    if (binary.size() == 1)
    {
        if (binary[0] == 1)
            return -1;
        else
            return 1;
    }
    else if (binary.size() == 2)
    {
        if (binary[1] == 1)
            value = 1;

        if (binary[0] == 1)
            return -value;
        return value;
    }

    const int beforeDecimalPoint = 0.75*binary.size();

    // Entire part
    int index = 0;
    for (unsigned int i = beforeDecimalPoint ; i >= 1 ; i--)
    {
        if (binary[i] == 1)
            value += power(2, index);
        index++;
    }

    // After decimal point part
    index = 1;
    for (int i = binary.size()-1 ; i > beforeDecimalPoint ; i--)
    {
        if (binary[i] == 1)
            value += power(2.0, -index);
        index++;
    }

    if (binary[0] == 1)
        value = -value;

    return value;
}

template <typename T>
T power(T nb, int pow)
{
    T result = 1;
    if (pow == 0)
        return result;
    else if (pow > 0)
    {
        for (int i = 1 ; i <= pow ; i++)
            result *= nb;
    }
    else
    {
        if (nb == 0)
            return std::numeric_limits<double>::max();

        for (int i = -1 ; i >= pow ; i--)
            result *= 1/nb;
    }

    return result;
}

#endif // GENERAL_H
