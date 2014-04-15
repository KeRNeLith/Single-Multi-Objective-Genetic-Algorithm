#ifndef SINGLEOBJECTIVEGA_H
#define SINGLEOBJECTIVEGA_H

#include "ga.h"

template<typename T, typename P, typename C>
/**
 * @brief The SingleObjectiveGA class extends GA to implement basic behaviour of single objective GA.
 */
class SingleObjectiveGA
        : public GA<T, P, C>
{
protected:
    bool m_elitism;             ///> Specify that we want to use elitism between each generation (conserve a percentage of chromosomes).

    /**
     * @brief runOneGeneration Do all steps needed for one generation (iteration), like mutation and crossover operators, etc...
     */
    virtual void runOneGeneration();

public:
    SingleObjectiveGA();

    virtual void initialize();
    virtual std::vector< C > performGA();
    virtual void reset();

    /**
     * @brief setElistism Enable/Disable the elitism selection.
     * @param elitismState State enable or disable of the elitism selection.
     */
    void setElistism(const bool elitismState) { m_elitism = elitismState; }
    /**
     * @brief getElistism Get the state of the elitism selection.
     * @return State of the elitism selection.
     */
    bool getElistism() { return m_elitism; }
};

template<typename T, typename P, typename C>
SingleObjectiveGA<T, P, C>::SingleObjectiveGA()
    : GA<T, P, C>()
    , m_elitism(true)
{
}

template<typename T, typename P, typename C>
void SingleObjectiveGA<T, P, C>::runOneGeneration()
{
    P* newPop = new P;
    while (!newPop->isFull())
    {
        if (m_elitism)
            newPop->addKeptChromosomes(this->m_population->getKeptChromosomes());
        C chromosome = this->m_population->crossOver(this->m_population->selectChromosomesPair());
        newPop->addChromosome(chromosome);
    }
    newPop->mutate();
    newPop->evaluateFitness();
    delete this->m_population;
    this->m_population = newPop;
    this->m_currentGeneration++;
}

template<typename T, typename P, typename C>
void SingleObjectiveGA<T, P, C>::initialize()
{
    if(this->m_isInitialized)
        return;

    this->generateRandomPopulation();
    this->m_population->evaluateFitness();
    this->m_currentGeneration = 1;
    this->m_isInitialized = true;
}

template<typename T, typename P, typename C>
std::vector<C> SingleObjectiveGA<T, P, C>::performGA()
{
    if (!this->m_isInitialized)
        throw std::runtime_error("GA not initialzed !");

    while (this->m_currentGeneration <= this->m_nbGenerationsWanted)
        runOneGeneration();

    return this->m_population->getBestSolution();
}

template<typename T, typename P, typename C>
void SingleObjectiveGA<T, P, C>::reset()
{
    this->m_currentGeneration = 1;
    this->m_isInitialized = false;
}

#endif // SINGLEOBJECTIVEGA_H
