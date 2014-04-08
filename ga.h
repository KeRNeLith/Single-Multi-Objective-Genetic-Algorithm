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

    Population<T, T2>* m_population;

    virtual void runOneGeneration();
    virtual void generateRandomPopulation();

public:
    GA();
    virtual ~GA();

    virtual void initialize();
    virtual T performGA();

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
    delete m_population;
}

template<typename T, typename T2>
void GA<T, T2>::initialize()
{
    generateRandomPopulation();
    m_population->evaluateFitness();
    m_isInitialized = true;
}

template<typename T, typename T2>
T GA<T, T2>::performGA()
{
    if (!m_isInitialized)
        return;

    while (m_currentGeneration < m_nbGenerationsWanted)
        runOneGeneration();

    return m_population->getBestSolution();
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
    Population<T, T2>* newPop;
    while (!newPop.isFull())
    {
        Chromosome<T, T2> chromosome = m_population->crossOver(m_population->selectChromosomesPair());
        newPop->addChromosome(chromosome);
    }
    newPop->mutate();
    newPop->evaluateFitness();
    delete m_population;
    m_population = newPop;
}

template<typename T, typename T2>
void GA<T, T2>::generateRandomPopulation()
{
    m_population = new Population<T, T2>();
    m_population->generateRandomChromosomes();
}

#endif // GA_H
