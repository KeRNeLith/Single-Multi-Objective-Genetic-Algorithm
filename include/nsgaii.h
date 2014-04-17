#ifndef NSGAII_H
#define NSGAII_H

#include "ga.h"

template<typename T, typename P, typename C>
class NSGAII
        : public GA<T, P, C>
{
protected:


    /**
     * @brief runOneGeneration Do all steps needed for one generation (iteration), like mutation and crossover operators, etc...
     */
    virtual void runOneGeneration();

    virtual P* breeding();

    virtual P* fastNonDominatedSort(P* popToSort);

    virtual void crowdingDistanceAssignement(P* popToAssignCrowdingDistance);

public:
    NSGAII();

    virtual void initialize();
    virtual std::vector< C > performGA();
    virtual void reset();
};

template<typename T, typename P, typename C>
NSGAII<T, P, C>::NSGAII()
    : GA<T, P, C>()
{
}
#include <iostream>
template<typename T, typename P, typename C>
void NSGAII<T, P, C>::runOneGeneration()
{
    // Execute breeding (create offspring using GA)
    P* offsprings = breeding();

    // Combine parent and offsprings population
    P* popParentOffsprings = new P;
    *popParentOffsprings = *offsprings;
    //std::cout << "dedans : " << popParentOffsprings->getCurrentNbChromosomes() << " | max size : " << popParentOffsprings->getNbMaxChromosomes() << std::endl;
    popParentOffsprings->add(*this->m_population);
    //std::cout << "dedans : " << popParentOffsprings->getCurrentNbChromosomes() << " | max size : " << popParentOffsprings->getNbMaxChromosomes() << std::endl;


    // TODO

    // TODO les delete

    this->m_currentGeneration++;
}

template<typename T, typename P, typename C>
P* NSGAII<T, P, C>::breeding()
{
    // TODO
    P* newPop = new P;
    newPop->generateRandomChromosomes();
    /*while (!newPop->isFull())
    {
        C chromosome = this->m_population->crossOver(this->m_population->selectChromosomesPair());
        newPop->addChromosome(chromosome);
    }
    newPop->mutate();
    newPop->evaluateFitness();*/

    return newPop;
}

template<typename T, typename P, typename C>
P* NSGAII<T, P, C>::fastNonDominatedSort(P* popToSort)
{
    return popToSort;
}

template<typename T, typename P, typename C>
void NSGAII<T, P, C>::crowdingDistanceAssignement(P* popToAssignCrowdingDistance)
{

}

template<typename T, typename P, typename C>
void NSGAII<T, P, C>::initialize()
{
    if(this->m_isInitialized)   // already initialized
        return;

    // Generate a random population make step that need to be done before running algorithm
    this->releaseMemory();
    this->m_population = new P;
    this->m_population->generateRandomChromosomes();
    //this->m_population->evaluateFitness(); TODO
    this->m_currentGeneration = 1;
    this->m_isInitialized = true;
}

template<typename T, typename P, typename C>
std::vector< C > NSGAII<T, P, C>::performGA()
{
    // Run the algorithm if it is initialized, and return Pareto Optimal Set
    if (!this->m_isInitialized)
        throw std::runtime_error("GA not initialzed !");

    while (this->m_currentGeneration <= this->m_nbGenerationsWanted)
        runOneGeneration();

    // TODO
    //return this->m_population->getBestSolution();

    return std::vector < C >();
}

template<typename T, typename P, typename C>
void NSGAII<T, P, C>::reset()
{
    this->m_currentGeneration = 1;
    this->m_isInitialized = false;
}

#endif // NSGAII_H
