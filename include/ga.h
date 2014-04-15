#ifndef GA_H
#define GA_H

#include <vector>

#include "population.h"

template<typename T, typename P, typename C>
/**
 * @brief The GA class Provide some prototypes of function to run genetic algorithms.
 */
class GA
{
protected:
    bool m_isInitialized;       ///> Indicate if the GA has been initialized.

    int m_nbGenerationsWanted;  ///> Number of generations (iterations) that wil be run in performGA function.
    int m_currentGeneration;    ///> Index of the current generation running.

    P* m_population;            ///> Current population of solutions.

    /**
     * @brief generateRandomPopulation Generate a random Population, use mainly at the beginning of the algorithm.
     */
    virtual void generateRandomPopulation();

    /**
     * @brief releaseMemory Free all the memory used by the algorithm that haven't been deallocate during the processing.
     */
    virtual void releaseMemory();

public:
    GA();
    virtual ~GA();

    /**
     * @brief initialize Do all things that are needed to do before running the algorithm.
     */
    virtual void initialize() =0;

    /**
     * @brief performGA Run the genetic algorithm.
     * @return the best solution found.
     */
    virtual std::vector< C > performGA() =0;

    /**
     * @brief reset Reset all values in order to do another run of the algorithm.
     */
    virtual void reset() =0;

    ////////////// Accessors/Setters //////////////
    /**
     * @brief setNbGenerationsWanted Set the number of generation that will be run to nbGenerationsWanted.
     * @param nbGenerationsWanted number of generations wanted.
     */
    void setNbGenerationsWanted(const int nbGenerationsWanted) { m_nbGenerationsWanted = nbGenerationsWanted; }

    /**
     * @brief getNbGenerationsWanted Get the number of generation that will be run to nbGenerationsWanted.
     * @return the number of generations wanted for the run.
     */
    int getNbGenerationsWanted() { return m_nbGenerationsWanted; }

    /**
     * @brief getIndexCurrentGeneration Get the index of the current generation.
     * @return the index of the current generation.
     */
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
