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

template<typename T, typename P, typename C>
void NSGAII<T, P, C>::runOneGeneration()
{
    // TODO
    /*P* newPop = new P;
    while (!newPop->isFull())
    {
        C chromosome = this->m_population->crossOver(this->m_population->selectChromosomesPair());
        newPop->addChromosome(chromosome);
    }
    newPop->mutate();
    newPop->evaluateFitness();
    delete this->m_population;
    this->m_population = newPop;*/


    this->m_currentGeneration++;
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

#include <iostream>
template<typename T, typename P, typename C>
void NSGAII<T, P, C>::initialize()
{
    if(this->m_isInitialized)   // already initialized
        return;

    // TODO
    P* pop = new P;
    std::cout << "size : " << pop->getCurrentNbChromosomes() << " | full : " << (pop->isFull() ? "true" : "false") << " | max possible : " << pop->getNbMaxChromosomes()  << " | max possible (tous) : " << pop->getSNbMaxChromosomes()<< std::endl;
    pop->setNbMaxChromosomes(150);
    std::cout << "size : " << pop->getCurrentNbChromosomes() << " | full : " << (pop->isFull() ? "true" : "false")  << " | max possible : " << pop->getNbMaxChromosomes()  << " | max possible (tous) : " << pop->getSNbMaxChromosomes()<< std::endl;
    pop->generateRandomChromosomes();
    std::cout << "size : " << pop->getCurrentNbChromosomes() << " | full : " << (pop->isFull() ? "true" : "false")  << " | max possible : " << pop->getNbMaxChromosomes()  << " | max possible (tous) : " << pop->getSNbMaxChromosomes()<< std::endl;

    /*this->m_population = new P;
    std::cout << "size : " << this->m_population->getCurrentNbChromosomes() << " | full : " << (this->m_population->isFull() ? "true" : "false") << " | max possible : " << this->m_population->getNbMaxChromosomes()  << " | max possible (tous) : " << this->m_population->getSNbMaxChromosomes()<< std::endl;
    this->m_population->setNbMaxChromosomes(150);
    std::cout << "size : " << this->m_population->getCurrentNbChromosomes() << " | full : " << (this->m_population->isFull() ? "true" : "false")  << " | max possible : " << this->m_population->getNbMaxChromosomes()  << " | max possible (tous) : " << this->m_population->getSNbMaxChromosomes()<< std::endl;
    this->generateRandomPopulation();
    std::cout << "size : " << this->m_population->getCurrentNbChromosomes() << " | full : " << (this->m_population->isFull() ? "true" : "false")  << " | max possible : " << this->m_population->getNbMaxChromosomes()  << " | max possible (tous) : " << this->m_population->getSNbMaxChromosomes()<< std::endl;
    */

    // Generate a random population make step that need to be done before running algorithm
    /*this->generateRandomPopulation();
    this->m_population->evaluateFitness();
    this->m_currentGeneration = 1;*/
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
