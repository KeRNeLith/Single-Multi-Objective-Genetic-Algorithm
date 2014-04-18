#ifndef CHROMOSOMEMINTINT_H
#define CHROMOSOMEMINTINT_H

#include "General.h"
#include "chromosome.h"
#include "population.h"

class ChromosomeMIntInt
        : public Chromosome<int, int>
{
protected:
    double m_crowdingDistance;                          ///> Crowding distance (to measure how solutions are crowed).
    int m_rank;                                         ///> Rank of the solution (or front).
    std::vector<ChromosomeMIntInt> m_dominatedSolutions;///> Solution dominated by this one.
    int m_nbSolutionDominatesMe;                        ///> Number of solutions that dominates this solution.

public:
    ChromosomeMIntInt();

    /**
     * @brief resetDominance Reset Values of m_dominatedSolutions and m_nbSolutionDominatesMe.
     */
    virtual void resetDominance();
    /**
     * @brief dominates Check which solution dominates the other.
     * @param other Solution that will be checked.
     * @return true if current solution dominates other, otherwise false.
     */
    virtual bool dominates(const ChromosomeMIntInt& other);

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
     * @brief getRank Get the Rank of the chromosome.
     * @return Rank.
     */
    virtual int getRank() const { return m_rank; }
    /**
     * @brief setRank Set the Rank of the chromosome.
     * @param rank Rank wanted.
     */
    virtual void setRank(const int rank) { m_rank = rank; }

    /**
     * @brief getDominatedSolution Solutions dominated by this chromosome.
     * @return Vector of solutions dominated by this chromosome.
     */
    virtual std::vector<ChromosomeMIntInt> getDominatedSolution() { return m_dominatedSolutions; }

    /**
     * @brief addDominatedSolution Add a Chromosome to the vector of solutions dominated.
     * @param other Chromosome to add.
     */
    virtual void addDominatedSolution(const ChromosomeMIntInt& other);

    /**
     * @brief setNbSolutionDominatesMe Set the number of solutions that dominates this one.
     * @param nb Number of solutions that dominates this one.
     */
    virtual void setNbSolutionDominatesMe(const int nb) { m_nbSolutionDominatesMe = nb; }

    /**
     * @brief getNbSolutionDominatesMe Get the number of solutions that dominates this one.
     * @return Number of solutions that dominates this one.
     */
    virtual int getNbSolutionDominatesMe() { return m_nbSolutionDominatesMe; }
};

#endif // CHROMOSOMEMINTINT_H
