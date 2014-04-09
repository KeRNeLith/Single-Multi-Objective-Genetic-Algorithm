#ifndef GA_H
#define GA_H

#include "population.h"

template<typename T, typename P, typename C>
class GA
{
protected:
    bool m_isInitialized;
    int m_nbGenerationsWanted;
    int m_currentGeneration;

    P* m_population;

    virtual void runOneGeneration();
    virtual void generateRandomPopulation();
    virtual void releaseMemory();

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

template<typename T, typename P, typename C>
GA<T, P, C>::GA()
    : m_isInitialized(false)
    , m_nbGenerationsWanted(100)
    , m_population(nullptr)
{
}

template<typename T, typename P, typename C>
GA<T, P, C>::~GA()
{
    releaseMemory();
}

template<typename T, typename P, typename C>
void GA<T, P, C>::initialize()
{
    generateRandomPopulation();
    m_population->evaluateFitness();
    m_currentGeneration = 1;
    m_isInitialized = true;
}

template<typename T, typename P, typename C>
T GA<T, P, C>::performGA()
{
    if (!m_isInitialized)
        throw std::runtime_error("GA not initialzed !");

    while (m_currentGeneration < m_nbGenerationsWanted)
        runOneGeneration();

    return m_population->getBestSolution();
}

template<typename T, typename P, typename C>
void GA<T, P, C>::reset()
{
    m_currentGeneration = 1;
    m_isInitialized = false;
}

template<typename T, typename P, typename C>
void GA<T, P, C>::runOneGeneration()
{
    P* newPop = new P;
    while (!newPop->isFull())
    {
        newPop->addKeptChromosomes(m_population->getKeptChromosomes());
        C chromosome = m_population->crossOver(m_population->selectChromosomesPair());
        newPop->addChromosome(chromosome);
    }
    newPop->mutate();
    newPop->evaluateFitness();
    delete m_population;
    m_population = newPop;
    m_currentGeneration++;
}

template<typename T, typename P, typename C>
void GA<T, P, C>::generateRandomPopulation()
{
    releaseMemory();
    m_population = new P();
    m_population->generateRandomChromosomes();
}

template<typename T, typename P, typename C>
void GA<T, P, C>::releaseMemory()
{
    if (m_population)
        delete m_population;
    m_population = nullptr;
}

#endif // GA_H
