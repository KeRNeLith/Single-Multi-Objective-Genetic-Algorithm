#ifndef POPULATION_H
#define POPULATION_H

#include <vector>
#include <algorithm>

#include "chromosome.h"

template<typename T, typename T2>
class Population
{
protected:
    // Counters
    static unsigned int m_nbMaxChromosomes;///> Number maximum of member for a population of chromosomes

    static double m_crossOverProbability;  ///> Probability for a chromosome to crossover
    static double m_mutateProbability;     ///> Probability for a chromosome to mutate

    std::vector< Chromosome<T, T2> > m_chromosomes;     ///> Chromosomes composing the population
    std::vector< T > m_cumulatedFitness;                ///> Store the cumulated fitness

    // Function of comparison
    bool smallerToHigher(const T& param1, const T& param2);

public:
    Population();
    ~Population();

    /**
     * @brief Evaluate the fitness for all chromosomes of the population
     * The function must rank all solutions according to their fitness in order : the worst to the better
     */
    virtual void evaluateFitness();

    /**
     * @brief Compute mutation for all chromosomes of the population according to m_mutateProbability
     */
    virtual void mutate();

    /**
     * @brief select a pair of Chromosomes
     * @return the pair of chromosomes chose as parent
     */
    virtual std::pair< Chromosome<T, T2>, Chromosome<T, T2> > selectChromosomesPair() =0;
    /**
     * @brief perform a crossOver on chromosomes according to m_crossOverProbability
     * @param parents pair of chromosomes that will be use as parents
     * @return children Chromosomes generated
     */
    virtual Chromosome<T, T2> crossOver(std::pair< Chromosome<T, T2>, Chromosome<T, T2> > parents) =0;

    /**
     * @brief generateRandomChromosomes generate a random population of chromosomes
     */
    void generateRandomChromosomes() =0;

    /**
     * @brief addChromosome add a Chromosome to m_chromosomes vector
     * @param chromosome will be add to m_chromosomes vector
     */
    void addChromosome(Chromosome<T, T2> chromosome);

    /**
     * @brief isFull
     * @return true if the population is full (m_chromosomes.size() >= m_nbMaxChromosomes)
     */
    bool isFull() { return m_chromosomes.size() >= m_nbMaxChromosomes; }

    ////////////// Accessors/Setters //////////////
    /**
     * @brief setNbMaxChromosomes set the number max of chromosomes for a population
     * @param nbMaxChromosomes number max
     */
    static void setNbMaxChromosomes(const int nbMaxChromosomes) { m_nbMaxChromosomes = nbMaxChromosomes; }
    /**
     * @brief getNbMaxChromosomes the numbr max of chromosomes
     * @return number max of chromosomes for the population
     */
    static int getNbMaxChromosomes() { return m_nbMaxChromosomes; }

    /**
     * @brief getCurrentNbChromosomes
     * @return number of members currently in the population
     */
    int getCurrentNbChromosomes() { return m_chromosomes.size(); }

    /**
     * @brief setCrossOverProbability set the probability to crossover
     * @param crossOverProbability probability to crossover
     */
    static void setCrossOverProbability(const double crossOverProbability) { m_crossOverProbability = crossOverProbability; }
    /**
     * @brief getCrossOverProbability get the probability to crossover
     * @return the probability to crossover
     */
    static double getCrossOverProbability() { return m_crossOverProbability; }

    T getBestSolution();
};


template<typename T, typename T2>
Population<T, T2>::Population()
{
}

template<typename T, typename T2>
Population<T, T2>::~Population()
{
}

template<typename T, typename T2>
void Population<T, T2>::evaluateFitness()
{
    for (unsigned int i = 0 ; i < m_chromosomes.size() ; i++)
        m_chromosomes[i].computeFitness();

    std::sort(m_chromosomes.begin(), m_chromosomes.end(), smallerToHigher);

    m_cumulatedFitness.clear();
    m_cumulatedFitness[0] = m_chromosomes[0].getFitness();
    for (unsigned int i = 1 ; i < m_chromosomes.size() ; i++)
        m_cumulatedFitness[i] = m_cumulatedFitness[i-1] + m_chromosomes[i].getFitness();
}

template<typename T, typename T2>
void Population<T, T2>::mutate()
{
    for (unsigned int i = 0 ; i < m_chromosomes.size() ; i++)
        m_chromosomes[i].mutate();
}

template<typename T, typename T2>
void Population<T, T2>::addChromosome(Chromosome<T, T2> chromosome)
{
    m_chromosomes.push_back(chromosome);
}

template<typename T, typename T2>
T Population<T, T2>::getBestSolution()
{
    return m_chromosomes[m_chromosomes.size()-1].getFitness();
}

template<typename T, typename T2>
bool Population<T, T2>::smallerToHigher(const T& param1, const T& param2)
{
    return param1 < param2;
}

#endif // POPULATION_H
