#ifndef GA_H
#define GA_H

#include "population.h"

template<typename T, typename T2>
class GA
{
protected:
    bool m_isInitialized;
    int m_nbGenerationsWanted;
    int m_currentGeneration;

    Population<T, T2> m_population;

    virtual void runOneGeneration();
    virtual void generateRandomPopulation();

public:
    GA();
    virtual ~GA();

    virtual void initialize();
    virtual void performGA();

    virtual void reset();

    ////////////// Accessors/Setters //////////////
    void setNbGenerationsWanted(const int nbGenerationsWanted) { m_nbGenerationsWanted = nbGenerationsWanted; }
    int getNbGenerationsWanted() { return m_nbGenerationsWanted; }

    int getIndexCurrentGeneration() { return m_currentGeneration; }
};

template<typename T, typename T2>
GA<T, T2>::GA()
    : m_isInitialized(false)
    , m_nbGenerationsWanted(100)
{
}

template<typename T, typename T2>
GA<T, T2>::~GA()
{
}

template<typename T, typename T2>
void GA<T, T2>::initialize()
{
    generateRandomPopulation();
    m_isInitialized = true;
}

template<typename T, typename T2>
void GA<T, T2>::performGA()
{
    if (!m_isInitialized)
        return;

    while (m_currentGeneration < m_nbGenerationsWanted)
        runOneGeneration();

    // TODO un return ?
}

template<typename T, typename T2>
void GA<T, T2>::reset()
{
    m_currentGeneration = 1;
    m_isInitialized = false;
}

template<typename T, typename T2>
void GA<T, T2>::runOneGeneration()
{
    // TODO
}

#endif // GA_H
