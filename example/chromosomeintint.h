#ifndef CHROMOSOMEINTINT_H
#define CHROMOSOMEINTINT_H

#include <string>
#include <sstream>

#include "General.h"
#include "chromosome.h"
#include "population.h"

class ChromosomeIntInt
        : public Chromosome<int, int>
{
public:
    ChromosomeIntInt();

    virtual void mutate();
    virtual void computeFitness();
    virtual void generateRandomChromosome();

    bool operator <(const ChromosomeIntInt& chromosome) const { return Chromosome<int, int>::operator <(chromosome);}
};

#endif // CHROMOSOMEINTINT_H
