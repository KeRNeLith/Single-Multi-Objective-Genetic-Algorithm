#ifndef POPULATION_H
#define POPULATION_H

#include <vector>
#include <algorithm>

#include "chromosome.h"
#include "General.h"

template<typename T, typename T2, typename C>
class Population
{
protected:
    // Counters
    static unsigned int m_nbMaxChromosomes;///> Number maximum of member for a population of chromosomes

    static double m_crossOverProbability;  ///> Probability for a chromosome to crossover
    static double m_mutateProbability;     ///> Probability for a chromosome to mutate

    std::vector< C > m_chromosomes;         ///> Chromosomes composing the population

public:
    Population();
    virtual ~Population();

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
    virtual std::pair< C, C > selectChromosomesPair() =0;
    /**
     * @brief perform a crossOver on chromosomes according to m_crossOverProbability
     * @param parents pair of chromosomes that will be use as parents
     * @return children Chromosomes generated
     */
    virtual C crossOver(std::pair< C, C > parents) =0;

    /**
     * @brief generateRandomChromosomes generate a random population of chromosomes
     */
    virtual void generateRandomChromosomes();

    /**
     * @brief addChromosome add a Chromosome to m_chromosomes vector
     * @param chromosome will be add to m_chromosomes vector
     */
    void addChromosome(C chromosome);

    /**
     * @brief isFull
     * @return true if the population is full (m_chromosomes.size() >= m_nbMaxChromosomes)
     */
    bool isFull();

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
    int getCurrentNbChromosomes();

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

    /**
     * @brief setMutateProbability set the probability to mutate
     * @param mutateProbability probability to mutate
     */
    static void setMutateProbability(const double mutateProbability) { m_mutateProbability = mutateProbability; }
    /**
     * @brief getMutateProbability get the probability to mutate
     * @return the probability to mutate
     */
    static double getMutateProbability() { return m_mutateProbability; }

    /**
     * @brief getBestSolution best solution found by GA
     * @return the solution to the problem
     */
    T getBestSolution() const;
};

template<typename T, typename T2, typename C>
unsigned int Population<T, T2, C>::m_nbMaxChromosomes = 100;

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

#include <iostream>
template<typename T, typename T2, typename C>
void Population<T, T2, C>::evaluateFitness()
{
    for (unsigned int i = 0 ; i < m_chromosomes.size() ; i++)
        m_chromosomes[i].computeFitness();

    // Sort chromosomes to have m_chromosomes[0] with the lower fitness
    // and m_chromosomes[m_chromosomes.size()] with the hightest
    std::sort(m_chromosomes.begin(), m_chromosomes.end(), smallerToHigher<C>);
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

template<typename T, typename T2, typename C>
T Population<T, T2, C>::getBestSolution() const
{
    if (m_chromosomes.empty())
        throw std::runtime_error("No Solution found !");

    return m_chromosomes[m_chromosomes.size()-1].getFitness();
}

#endif // POPULATION_H
