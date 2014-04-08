#ifndef POPULATION_H
#define POPULATION_H

#include <vector>

#include "chromosome.h"

template<typename T, typename T2>
class Population
{
protected:
    // Counters
    static unsigned int m_nbMaxChromosomes;///> Number maximum of member for a population of chromosomes

    static double m_crossOverProbability;  ///> Probability for a chromosome to crossover
    static double m_mutateProbability;     ///> Probability for a chromosome to mutate

    std::vector< Chromosome<T, T2> > m_chromosomes;  ///> Chromosomes composing the population

    /**
     * @brief select a pair of Chromosomes
     * @return the pair of chromosomes chose as parent
     */
    virtual std::pair< Chromosome<T, T2>, Chromosome<T, T2> > selectChromosomesPair() =0;

public:
    Population();
    ~Population();

    /**
     * @brief Evaluate the fitness for all chromosomes of the population
     */
    virtual void evaluateFitness();

    /**
     * @brief Compute mutation for all chromosomes of the population according to m_mutateProbability
     */
    virtual void mutate();

    /**
     * @brief perform a crossOver on chromosomes according to m_crossOverProbability
     * @param parents pair of chromosomes that will be use as parents
     * @return children Chromosomes generated
     */
    virtual std::pair< Chromosome<T, T2>, Chromosome<T, T2> > crossOver(std::pair< Chromosome<T, T2>, Chromosome<T, T2> > parents) =0;


    /**
     * @brief isFull
     * @return true if the population is full (m_chromosomes.size() >= m_nbMaxChromosomes)
     */
    bool isFull() { return m_chromosomes.size() >= m_nbMaxChromosomes; }

    ////////////// Accessors/Setters //////////////
    static void setNbMaxChromosomes(const int nbMaxChromosomes) { m_nbMaxChromosomes = nbMaxChromosomes; }
    static int getNbMaxChromosomes() { return m_nbMaxChromosomes; }

    int getCurrentNbChromosomes() { return m_chromosomes.size(); }

    static void setCrossOverProbability(const double crossOverProbability) { m_crossOverProbability = crossOverProbability; }
    static double getCrossOverProbability() { return m_crossOverProbability; }
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
    // TODO
}

template<typename T, typename T2>
void Population<T, T2>::mutate()
{
    // TODO
}

template<typename T, typename T2>
std::pair<Chromosome<T, T2>, Chromosome<T, T2> > Population<T, T2>::selectChromosomesPair()
{
    // TODO
}

template<typename T, typename T2>
std::pair< Chromosome<T, T2>, Chromosome<T, T2> > Population<T, T2>::crossOver(std::pair<Chromosome<T, T2>, Chromosome<T, T2> > parents)
{
    // TODO
}

#endif // POPULATION_H
