#ifndef NSGAII_H
#define NSGAII_H

#include "ga.h"

template<typename T, typename P, typename C>
class NSGAII
        : public GA<T, P, C>
{
protected:

    P* m_offspring; ///> Offspring of m_population.

    virtual void releaseMemory();

    /**
     * @brief runOneGeneration Do all steps needed for one generation (iteration), like mutation and crossover operators, etc...
     */
    virtual void runOneGeneration();

    /**
     * @brief breeding Perform GA operators (selection, crossover, mutation) on m_population.
     * @return The offspring population.
     */
    virtual P* breeding();

    /**
     * @brief fastNonDominatedSort Determine all non-dominated front of the Population in parameter.
     * @param popToSort Population to search non-dominated fronts.
     * @return Vector of non-dominated fronts, fisrt element correspond to front 1 and so on.
     */
    virtual std::vector < P > fastNonDominatedSort(P* popToSort);

    /**
     * @brief crowdingDistanceAssignement Compute the crowding distance on the popualtion in parameter.
     * @param popToAssignCrowdingDistance Population which we want to compute crowding distance.
     */
    virtual void crowdingDistanceAssignement(P* popToAssignCrowdingDistance);

public:
    NSGAII();
    virtual ~NSGAII();

    virtual void initialize();
    virtual std::vector< C > performGA();
    virtual void reset();
};

template<typename T, typename P, typename C>
NSGAII<T, P, C>::NSGAII()
    : GA<T, P, C>()
    , m_offspring(nullptr)
{
}

template<typename T, typename P, typename C>
NSGAII<T, P, C>::~NSGAII()
{
    releaseMemory();
}

template<typename T, typename P, typename C>
void NSGAII<T, P, C>::releaseMemory()
{
    GA<T, P, C>::releaseMemory();
    if (m_offspring)
        delete m_offspring;
    m_offspring = nullptr;
}

template<typename T, typename P, typename C>
void NSGAII<T, P, C>::runOneGeneration()
{
    // Combine parent and offsprings population
    this->m_population->add(*m_offspring);

    // Determine all non dominated fronts
    std::vector < P > fronts = fastNonDominatedSort(this->m_population);

    // Secure check if it's not empty (should never arrived)
    if (fronts.empty())
    {
        this->m_currentGeneration++;
        return;
    }

    P* newParents = new P;  // Future population
    int i = 0;              // Index front
    // Until the population is filled
    while (newParents->getCurrentNbChromosomes() + fronts[i].getCurrentNbChromosomes() <= newParents->getNbMaxChromosomes())
    {
        // Calculate crowding-distance in ith Front
        crowdingDistanceAssignement(&fronts[i]);
        // Include ith non-dominated front in the population
        newParents->addChromosomes(fronts[i].getChromosomes());
        i++;    // Check the next front for inclusion
    }

    // Sort in descending order the ith front using crowding operator
    std::sort(fronts[i].getChromosomes().begin(), fronts[i].getChromosomes().end(), crowdingOperator<C>);

    // Choose the first (max chromosomes - size newParents) of ith front
    delete this->m_population;
    this->m_population = new P;
    newParents->addChromosomes(fronts[i].getChromosomes(), newParents->getNbMaxChromosomes() - newParents->getCurrentNbChromosomes());
    *this->m_population = *newParents;
    // Use selection, crossover and mutation to create new offspring population
    delete m_offspring;
    // Execute breeding (create offspring using GA)
    m_offspring = breeding();

    this->m_currentGeneration++;    // Generation counter
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
std::vector<P> NSGAII<T, P, C>::fastNonDominatedSort(P* popToSort)
{
    // TODO
    return std::vector<P>();
}

template<typename T, typename P, typename C>
void NSGAII<T, P, C>::crowdingDistanceAssignement(P* popToAssignCrowdingDistance)
{
    // TODO
}

template<typename T, typename P, typename C>
void NSGAII<T, P, C>::initialize()
{
    // TODO
    if(this->m_isInitialized)   // already initialized
        return;

    this->releaseMemory();

    // Generate a random population make step that need to be done before running algorithm
    this->m_population = new P;
    this->m_population->generateRandomChromosomes();
    this->m_population->evaluateFitness();

    // Create offspring of the random population
    this->m_offspring = new P;
    this->m_offspring->generateRandomChromosomes(); // TO CHANGE
    this->m_offspring->evaluateFitness();

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
