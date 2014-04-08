#ifndef CHROMOSOME_H
#define CHROMOSOME_H

#include <functional>

template<typename T, typename T2>
class Chromosome
{
protected:
    T m_fitness;    ///> Fitness of the individual for the given problem
    T2 m_datas;     ///> Describe how the chromosome is represented to apply the GA

public:
    Chromosome();
    virtual ~Chromosome();

    virtual void mutate() =0;
    virtual void computeFitness() =0;

    ////////////// Accessor/Setters //////////////
    virtual T getFitness() { return m_fitness; }
    virtual T2 getDatas() { return m_datas; }
};



template<typename T, typename T2>
Chromosome<T, T2>::Chromosome()
{
}

template<typename T, typename T2>
Chromosome<T, T2>::~Chromosome()
{
}

#endif // CHROMOSOME_H
