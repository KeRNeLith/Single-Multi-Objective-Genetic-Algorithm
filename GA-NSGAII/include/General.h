#ifndef GENERAL_H
#define GENERAL_H

#include <iostream>
#include <chrono>
#include <random>
#include <locale>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <limits>

/////////////////////////////////////////////////////////////
//////////////////////// Declarations ///////////////////////
/////////////////////////////////////////////////////////////

template<typename T>
/**
 * @brief The Less struct Compare the 2 parameters.
 */
struct Less
{
    /**
     * @brief operator() Compare the 2 parameters.
     * @param param1 First Operand.
     * @param param2 Second Operand.
     * @return true if param1 < param2, otherwise return false.
     */
    bool operator()(const T& param1, const T& param2) const
    {
        return param1 < param2;
    }
};

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
T getNumber(const std::string& s);

template<typename T>
/**
 * @brief The CrowdingOperator struct Compare the 2 parameters according to their crowding distance and rank.
 */
struct CrowdingOperator
{
    /**
     * @brief operator() Compare the 2 parameters according to their crowding distance and rank.
     * @param param1 First Operand.
     * @param param2 Second Operand.
     * @return true if (rank param1 < rank param2) or (rank param1 = rank param2 and distance param1 > distance param2), otherwise return false.
     */
    bool operator()(const T& param1, const T& param2) const
    {
        return (param1.getRank() < param2.getRank())
                || (param1.getRank() == param2.getRank() && param1.getDistance() > param2.getDistance());
    }
};

template<typename T>
/**
 * @brief The Ascending struct Use to perform comparisons pair by pair in ascending order.
 */
struct Ascending
{
    int index;  ///> Array index
    /**
     * @return param1.getFitness()[index] < param2.getFitness()[index]
     */
    bool operator()(const T& param1, const T& param2) const
    {
        return param1.getFitness()[index] < param2.getFitness()[index];
    }
};

template<typename T>
/**
 * @brief The Descending struct Use to perform comparisons pair by pair in descending order.
 */
struct Descending
{
    int index;  ///> Array index
    /**
     * @return param1.getFitness()[index] > param2.getFitness()[index]
     */
    bool operator()(const T& param1, const T& param2) const
    {
        return param1.getFitness()[index] > param2.getFitness()[index];
    }
};

template<typename T>
/**
 * @brief The EmptyPopulation struct Check if the popualtion is empty.
 */
struct EmptyPopulation
{
    /**
     * @brief operator() Check if the popualtion is empty.
     * @param param Population to check.
     * @return true if it's empty, otherwise return false.
     */
    bool operator()(const T& param) const
    {
        return param.getCurrentNbChromosomes() == 0;
    }
};

template <typename T>
/**
 * @brief getIntegerFromBinary Compute the integer decimal number corresponding to the parameter.
 * @param binary Vector of bits.
 * @return Integer decimal number.
 */
int getIntegerFromBinary(const std::vector<T> binary);

template <typename T>
/**
 * @brief getUnsignedIntegerFromBinary Compute the unsigned integer decimal number corresponding to the parameter.
 * @param binary Vector of bits.
 * @return Unsigned integer decimal number.
 */
unsigned int getUnsignedIntegerFromBinary(const std::vector<T> binary);

template <typename T>
/**
 * @brief getDoubleFromBinary Compute the double decimal number corresponding to the parameter.
 * @param binary Vector of bits (first 60% of bits correspond to the entire part of the number).
 * @return Double decimal number.
 */
double getDoubleFromBinary(const std::vector<T> binary);

/**
 * @brief power Compute the number nb^pow.
 * @param nb Number we want the power.
 * @param pow Power wanted.
 * @return nb^pow.
 */
template <typename T>
T power(const T nb, const int pow);

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

class PairMinMax
{
public:
    int index;
    std::pair<double, double> m_minMax;

    void operator()(const std::vector<double>& param)
    {
        m_minMax = std::pair<double, double>(std::min(param[index], m_minMax.first), std::max(param[index], m_minMax.second));
    }
};

/**
 * @brief Compute the truncate floor value of number value.
 * @param value Number that will be floor and truncate.
 * @param nbDecimal Number of decimal you want to keep.
 * @return The truncate floor value.
 */
template <typename T>
inline T floorValue(const T& value, int nbDecimal);

template <typename T>
T extractNumberFromStr(const std::string& str);

// Random number generator
// Other possible seed : std::chrono::system_clock::now().time_since_epoch().count()
static std::mt19937 generator(686452231);

/////////////////////////////////////////////////////////////
//////////////////////// Definitions ////////////////////////
/////////////////////////////////////////////////////////////

template<typename T>
std::basic_string<T> lowerCase(std::basic_string<T>& s)
{
    std::transform(s.begin(), s.end(), s.begin(), tolower);
    return s;
}

template<typename T>
T getNumber(const std::string& s)
{
    std::stringstream ss(s);
    T ret;
    return ss >> ret ? ret : std::numeric_limits<T>::max();
}

template <typename T>
int getIntegerFromBinary(const std::vector<T> binary)
{
    int value = 0;
    int index = 0;
    for (unsigned int i = binary.size()-1 ; i >= 1 ; --i)
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
unsigned int getUnsignedIntegerFromBinary(const std::vector<T> binary)
{
    unsigned int value = 0;
    int index = 0;
    for (int i = binary.size()-1 ; i >= 0 ; --i)
    {
        if (binary[i] == 1)
            value += power(2, index);
        index++;
    }

    return value;
}

template <typename T>
double getDoubleFromBinary(const std::vector<T> binary)
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

    const unsigned int nbBits = binary.size();
    double value = 0;

    if (nbBits == 1)
    {
        if (binary[0] == 1)
            return -1;
        else
            return 1;
    }
    else if (nbBits == 2)
    {
        if (binary[1] == 1)
            value = 1;

        if (binary[0] == 1)
            return -value;
        return value;
    }

    const unsigned int beforeDecimalPoint = 0.6*nbBits;

    // Entire part
    int index = 0;
    for (unsigned int i = beforeDecimalPoint ; i >= 1 ; --i)
    {
        if (binary[i] == 1)
            value += power(2, index);
        index++;
    }

    // After decimal point part
    index = 1;
    for (int i = nbBits-1 ; i > beforeDecimalPoint ; --i)
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
T power(const T nb, const int pow)
{
    T result = 1;
    if (pow == 0)
        return result;
    else if (pow > 0)
    {
        for (int i = 1 ; i <= pow ; ++i)
            result *= nb;
    }
    else
    {
        if (nb == 0)
            return std::numeric_limits<double>::max();

        for (int i = -1 ; i >= pow ; --i)
            result *= 1/(double)nb;
    }

    return result;
}

template <typename T>
inline T floorValue(const T& value, int nbDecimal)
{
    if (nbDecimal < 0)
        nbDecimal = 0;
    double power = pow(10, nbDecimal);
    return (floor(value * power)) / (T)power;
}

template <typename T>
T extractNumberFromStr(const std::string& str)
{
    std::string tmpNumber;
    T number = 0;

    for (unsigned int i = 0 ; i < str.size() ; i++)
    {
        // iterate the string to find the first "number" character
        // if found create another loop to extract it
        // and then break the current one
        // extract the first encountered numeric block
        if (isdigit(str[i]))
        {
            for (unsigned int a=i; a<str.size(); a++)
            {
                tmpNumber += str[a];
            }
            //the first numeric block is extracted
            break;
        }
    }

    std::istringstream iss(tmpNumber);
    iss >> number;
    return number;
}

#endif // GENERAL_H
