#ifndef CHROMOSOME_H
#define CHROMOSOME_H

#include <vector>
#include <functional>

template<typename T, typename T2>
class Chromosome
{
protected:
    T m_fitness;    ///> Fitness of the individual for the given problem
    std::vector< T2> m_datas;           ///> Describe how the chromosome is represented to apply the GA
    static unsigned int m_nbGenes;      ///> Number of genes

public:
    Chromosome();
    virtual ~Chromosome();

    virtual void mutate() =0;
    virtual void computeFitness() =0;
    virtual void generateRandomChromosome() =0;

    ////////////// Accessor/Setters //////////////
    virtual T getFitness() const { return m_fitness; }

    virtual std::vector< T2 > getDatas() const { return m_datas; }
    virtual void setDatas(const std::vector< T2 > datas) { m_datas = datas; }

    static void setNbGenes(const int nbGenes) { m_nbGenes = nbGenes; }
    static int getNbGenes() { return m_nbGenes; }

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
