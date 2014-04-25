#ifndef SINGLEOBJECTIVEGA_H
#define SINGLEOBJECTIVEGA_H

#include <iostream>

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
    bool m_consoleDisplay;

    /**
     * @brief displayAdvancement Display in the console advancement of the algorithm.
     */
    virtual void displayAdvancement();

public:
    SingleObjectiveGA(bool consoleDisplay = true);

    virtual void initialize();
    virtual std::vector< C > performGA();
    virtual void reset();

    /**
     * @brief runOneGeneration Do all steps needed for one generation (iteration), like mutation and crossover operators, etc...
     */
    virtual void runOneGeneration();

    ////////////// Accessors/Setters //////////////
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

    /**
     * @brief setConsoleDisplay Enable/Disable console display of the advancement of algorithm.
     * @param state true if want display, else false.
     */
    virtual void setConsoleDisplay(bool state) { m_consoleDisplay = state; }
    /**
     * @brief getConsoleDsiplay Get state Enable/Disable of the console display of the advancement of algorithm.
     * @return Bool of the state (m_consoleDisplay).
     */
    virtual bool getConsoleDsiplay() const { return m_consoleDisplay; }
};

template<typename T, typename P, typename C>
SingleObjectiveGA<T, P, C>::SingleObjectiveGA(bool consoleDisplay)
    : GA<T, P, C>()
    , m_elitism(true)
    , m_consoleDisplay(consoleDisplay)
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

    // Only for Display
    if (m_consoleDisplay)
        displayAdvancement();

    this->m_currentGeneration++;
}

template<typename T, typename P, typename C>
void SingleObjectiveGA<T, P, C>::initialize()
{
    if(this->m_isInitialized)   // already initialized
        return;

    // Generate a random population make step that need to be done before running algorithm
    this->releaseMemory();
    this->m_population = new P;
    this->m_population->generateRandomChromosomes();
    this->m_population->evaluateFitness();
    this->m_currentGeneration = 1;
    this->m_isInitialized = true;
}

template<typename T, typename P, typename C>
std::vector<C> SingleObjectiveGA<T, P, C>::performGA()
{
    // Run the algorithm if it is initialized, and return the best solution found
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

template<typename T, typename P, typename C>
void SingleObjectiveGA<T, P, C>::displayAdvancement()
{
    const int nbSymbols = 40;
    double advancement = this->m_currentGeneration / (double)this->m_nbGenerationsWanted;
    double nbSymbolsToDraw = nbSymbols * advancement;
    std::cout << "\r" << "[";
    for (int i = 0 ; i < nbSymbols ; i++)
    {
        if (i <= nbSymbolsToDraw)
            std::cout << "=";
        else
            std::cout << " ";
    }
    std::cout << "]" << "\t" << advancement*100 << "%";
}

#endif // SINGLEOBJECTIVEGA_H
