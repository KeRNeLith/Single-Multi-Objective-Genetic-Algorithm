#ifndef POPULATION_H
#define POPULATION_H

#include <vector>
#include <algorithm>

#include "chromosome.h"
#include "General.h"

template<typename T, typename T2, typename C>
/**
 * @brief The Population class Provide the storage of multiple chromosomes in order to apply a GA.
 */
class Population
{
protected:
    // Counters
    static unsigned int m_nbMaxChromosomes;         ///> Number maximum of member for a population of chromosomes.
    static double m_proportionalChromosomesKeep;    ///> Indicate a percentage of chromosomes that wil be keep in each generation.

    static double m_crossOverProbability;           ///> Probability for a chromosome to crossover.
    static double m_mutateProbability;              ///> Probability for a chromosome to mutate.

    std::vector< C > m_chromosomes;                 ///> Chromosomes composing the population.

public:
    Population();
    virtual ~Population();

    /**
     * @brief Evaluate the fitness for all chromosomes of the population.
     * The function must rank all solutions according to their fitness in order : the worst to the better.
     */
    virtual void evaluateFitness() =0;

    /**
     * @brief Compute mutation for all chromosomes of the population according to m_mutateProbability.
     */
    virtual void mutate();

    /**
     * @brief select a pair of Chromosomes.
     * @return the pair of chromosomes chose as parent.
     */
    virtual std::pair< C, C > selectChromosomesPair() =0;
    /**
     * @brief perform a crossOver on chromosomes according to m_crossOverProbability.
     * @param parents pair of chromosomes that will be use as parents.
     * @return children Chromosomes generated.
     */
    virtual C crossOver(std::pair< C, C > parents) =0;

    /**
     * @brief generateRandomChromosomes generate a random population of chromosomes.
     */
    virtual void generateRandomChromosomes();

    /**
     * @brief addChromosome Add a Chromosome to m_chromosomes vector.
     * @param chromosome Will be add to m_chromosomes vector.
     */
    void addChromosome(C chromosome);

    /**
     * @brief isFull Check if the population is completely filled.
     * @return true if the population is full (m_chromosomes.size() >= m_nbMaxChromosomes).
     */
    bool isFull();

    ////////////// Accessors/Setters //////////////
    /**
     * @brief setNbMaxChromosomes Set the number max of chromosomes for a population.
     * @param nbMaxChromosomes Number max of chromosomes.
     */
    static void setNbMaxChromosomes(const int nbMaxChromosomes) { m_nbMaxChromosomes = nbMaxChromosomes; }
    /**
     * @brief getNbMaxChromosomes Get the numbr max of chromosomes.
     * @return Number max of chromosomes for the population.
     */
    static int getNbMaxChromosomes() { return m_nbMaxChromosomes; }

    /**
     * @brief getCurrentNbChromosomes Get the current number of chromosomes in the population.
     * @return Number of members currently in the population.
     */
    int getCurrentNbChromosomes();

    /**
     * @brief setCrossOverProbability Set the probability to crossover.
     * @param crossOverProbability Probability to crossover.
     */
    static void setCrossOverProbability(const double crossOverProbability) { m_crossOverProbability = crossOverProbability; }
    /**
     * @brief getCrossOverProbability Get the probability to crossover.
     * @return The probability to crossover.
     */
    static double getCrossOverProbability() { return m_crossOverProbability; }

    /**
     * @brief setMutateProbability Set the probability to mutate.
     * @param mutateProbability Probability to mutate.
     */
    static void setMutateProbability(const double mutateProbability) { m_mutateProbability = mutateProbability; }
    /**
     * @brief getMutateProbability Get the probability to mutate
     * @return The probability to mutate
     */
    static double getMutateProbability() { return m_mutateProbability; }

    /**
     * @brief setProportionalKeeping Set the coefficient of proportionnality for chromosomes that will be kept in the current generation for the next.
     * @param proportionalKeeping Coefficient of proportionnality.
     */
    static void setProportionalKeeping(const double proportionalKeeping) { m_proportionalChromosomesKeep = proportionalKeeping; if (m_proportionalChromosomesKeep > 1) m_proportionalChromosomesKeep = 1; if (m_proportionalChromosomesKeep < 0) m_proportionalChromosomesKeep = 0; }
    /**
     * @brief getProportionalKeeping Get the coefficient of proportionnality for chromosomes that will be kept in the current generation for the next.
     * @return Coefficient of proportionnality.
     */
    static double getProportionalKeeping() { return m_proportionalChromosomesKeep; }

    /**
     * @brief getBestSolution Best solution found by GA.
     * @return The solution to the problem.
     */
    virtual std::vector< C > getBestSolution() const =0;
};

// Init static variables
template<typename T, typename T2, typename C>
unsigned int Population<T, T2, C>::m_nbMaxChromosomes = 100;
template<typename T, typename T2, typename C>
double Population<T, T2, C>::m_proportionalChromosomesKeep = 0.2;

template<typename T, typename T2, typename C>
double Population<T, T2, C>::m_crossOverProbability = 0.3;
template<typename T, typename T2, typename C>
double Population<T, T2, C>::m_mutateProbability = 0.04;


template<typename T, typename T2, typename C>
Population<T, T2, C>::Population()
    : m_chromosomes()
{
}

template<typename T, typename T2, typename C>
Population<T, T2, C>::~Population()
{
}

template<typename T, typename T2, typename C>
void Population<T, T2, C>::mutate()
{
    for (unsigned int i = 0 ; i < m_chromosomes.size() ; i++)
        m_chromosomes[i].mutate();
}

template<typename T, typename T2, typename C>
void Population<T, T2, C>::generateRandomChromosomes()
{
    while (!isFull())
    {
        C chromosome;
        chromosome.generateRandomChromosome();
        addChromosome(chromosome);
    }
}

template<typename T, typename T2, typename C>
void Population<T, T2, C>::addChromosome(C chromosome)
{
    m_chromosomes.push_back(chromosome);
}

template<typename T, typename T2, typename C>
bool Population<T, T2, C>::isFull()
{
    if (m_chromosomes.empty())
        return false;
    return m_chromosomes.size() >= m_nbMaxChromosomes;
}

template<typename T, typename T2, typename C>
int Population<T, T2, C>::getCurrentNbChromosomes()
{
    if (m_chromosomes.empty())
        return 0;
    return m_chromosomes.size();
}

#endif // POPULATION_H
