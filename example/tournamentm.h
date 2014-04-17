#ifndef TOURNAMENTM_H
#define TOURNAMENTM_H

#include "population.h"

template<typename T, typename T2, typename C>
/**
 * @brief The RouletteWheel class Provide a selection of parents to breeding based on a Tournament and for Multi objective GA.
 */
class TournamentM
        : public Population<T, T2, C>
{
protected:

    virtual void destroy();
    virtual void copy(const TournamentM<T, T2, C> &other);

public:
    TournamentM(const int maxChromosome = -1);
    TournamentM(const TournamentM &other);

    virtual void evaluateFitness();
    virtual std::pair< C, C > selectChromosomesPair();
    virtual C crossOver(std::pair< C, C > parents);
    virtual std::vector< C > getBestSolution() const;

    // Operator Like
    TournamentM& add(const TournamentM& op);
    // Operator
    TournamentM& operator=(const TournamentM& other);
};

template<typename T, typename T2, typename C>
TournamentM<T, T2, C>::TournamentM(const int maxChromosome)
    : Population<T, T2, C>(maxChromosome == -1 ? this->m_sNbMaxChromosomes : maxChromosome)
{
}

template<typename T, typename T2, typename C>
TournamentM<T, T2, C>::TournamentM(const TournamentM& other)
    : Population<T, T2, C>(other)
{
    copy(other);
}

template<typename T, typename T2, typename C>
void TournamentM<T, T2, C>::destroy()
{
    Population<T, T2, C>::destroy();
}

template<typename T, typename T2, typename C>
void TournamentM<T, T2, C>::copy(const TournamentM<T, T2, C> &other)
{
    Population<T, T2, C>::copy(other);
}

template<typename T, typename T2, typename C>
void TournamentM<T, T2, C>::evaluateFitness()
{

}

template<typename T, typename T2, typename C>
std::pair< C, C > TournamentM<T, T2, C>::selectChromosomesPair()
{
    return std::pair< C, C >();
}

template<typename T, typename T2, typename C>
C TournamentM<T, T2, C>::crossOver(std::pair< C, C > parents)
{
    return C();
}

template<typename T, typename T2, typename C>
std::vector< C > TournamentM<T, T2, C>::getBestSolution() const
{
    return std::vector < C >();
}

template<typename T, typename T2, typename C>
TournamentM<T, T2, C>& TournamentM<T, T2, C>::add(const TournamentM& op)
{
    Population<T, T2, C>::add(op);

    evaluateFitness();

    return *this;
}

template<typename T, typename T2, typename C>
TournamentM<T, T2, C>& TournamentM<T, T2, C>::operator=(const TournamentM& other)
{
    if(this != dynamic_cast<const TournamentM<T, T2, C>*>(&other))
    {
        destroy();
        copy(other);
    }

    return *this;
}

#endif // TOURNAMENTM_H
