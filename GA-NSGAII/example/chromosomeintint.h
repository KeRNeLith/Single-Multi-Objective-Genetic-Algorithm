#ifndef CHROMOSOMEINTINT_H
#define CHROMOSOMEINTINT_H

#include "General.h"
#include "chromosome.h"
#include "population.h"

class ChromosomeIntInt
        : public smoga::Chromosome<int, int>
{
public:
    ChromosomeIntInt();

    virtual bool mutate();
    virtual void computeFitness();
    virtual void generateRandomChromosome();

    // Operator
    bool operator <(const ChromosomeIntInt& chromosome) const { return smoga::Chromosome<int, int>::operator <(chromosome);}
};

#endif // CHROMOSOMEINTINT_H
