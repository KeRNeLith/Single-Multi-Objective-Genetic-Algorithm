#ifndef CHROMOSOME_H
#define CHROMOSOME_H

#include <vector>
#include <functional>

template<typename T, typename T2>
/**
 * @brief The Chromosome class Provide some basic datas and functions to manage a Chromosome in GAs.
 */
class Chromosome
{
protected:
    T m_fitness;                        ///> Fitness of the individual for the given problem
    std::vector< T2> m_datas;           ///> Describe how the chromosome is represented to apply the GA
    static unsigned int m_nbGenes;      ///> Number of genes

public:
    Chromosome();
    virtual ~Chromosome();

    /**
     * @brief mutate Method call to apply mutation on the chromosome.
     */
    virtual void mutate() =0;

    /**
     * @brief computeFitness Compute the fitness of the chromosome.
     */
    virtual void computeFitness() =0;

    /**
     * @brief generateRandomChromosome Generate Random values for the chromosome.
     */
    virtual void generateRandomChromosome() =0;

    ////////////// Accessor/Setters //////////////
    /**
     * @brief getFitness Fitness of the chromosome.
     * @return The fitness.
     */
    virtual T getFitness() const { return m_fitness; }

    /**
     * @brief getDatas Allow to get all genes of the chromosome.
     * @return Genes's vector of the chromosome.
     */
    virtual std::vector< T2 > getDatas() const { return m_datas; }
    /**
     * @brief setDatas Set all genes of the chromosome to the parameter value.
     * @param datas Genes's vector of the chromosome
     */
    virtual void setDatas(const std::vector< T2 > datas) { m_datas = datas; }

    /**
     * @brief setNbGenes Set the number of genes composing the chromosome.
     * @param nbGenes The number of genes wanted.
     */
    static void setNbGenes(const int nbGenes) { m_nbGenes = nbGenes; }
    /**
     * @brief getNbGenes Get the number of genes composing the chromosome.
     * @return The number of genes.
     */
    static int getNbGenes() { return m_nbGenes; }

    // Operator
    bool operator<(const Chromosome<T, T2>& chromosome) const { return m_fitness < chromosome.m_fitness; }
};

template<typename T, typename T2>
unsigned int Chromosome<T, T2>::m_nbGenes = 10;

template<typename T, typename T2>
Chromosome<T, T2>::Chromosome()
{
}

template<typename T, typename T2>
Chromosome<T, T2>::~Chromosome()
{
}

#endif // CHROMOSOME_H
