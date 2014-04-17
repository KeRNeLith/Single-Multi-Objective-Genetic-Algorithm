#ifndef CHROMOSOMEMINTINT_H
#define CHROMOSOMEMINTINT_H

#include "chromosome.h"

class ChromosomeMIntInt
        : public Chromosome<int, int>
{
protected:
    double m_crowdingDistance;
    int m_rank;

public:
    ChromosomeMIntInt();

    virtual bool mutate();
    virtual void computeFitness();
    virtual void generateRandomChromosome();

    /**
     * @brief getDistance Get crowding distance of the chromosome.
     * @return Crowding distance.
     */
    virtual double getDistance() const { return m_crowdingDistance; }
    /**
     * @brief setDistance Set crowding distance of the chromosome.
     * @param dist Crowding distance.
     */
    virtual void setDistance(const double dist) { m_crowdingDistance = dist; }

    /**
     * @brief getRank Rank of the chromosome.
     * @return Rank
     */
    virtual int getRank() const { return m_rank; }
};

#endif // CHROMOSOMEMINTINT_H
