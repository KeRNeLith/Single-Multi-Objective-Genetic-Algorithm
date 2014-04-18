#include "chromosomemintint.h"

ChromosomeMIntInt::ChromosomeMIntInt()
    : Chromosome<int, int>()
    , m_crowdingDistance(0)
    , m_rank(-1)
    , m_dominatedSolutions()
    , m_nbSolutionDominatesMe(0)
{
}

void ChromosomeMIntInt::resetDominance()
{
    m_nbSolutionDominatesMe = 0;
    m_dominatedSolutions.clear();
}

bool ChromosomeMIntInt::dominates(const ChromosomeMIntInt& other)
{
    return false;
}

bool ChromosomeMIntInt::mutate()
{
    return false;
}

void ChromosomeMIntInt::computeFitness()
{

}

void ChromosomeMIntInt::generateRandomChromosome()
{

}

void ChromosomeMIntInt::addDominatedSolution(const ChromosomeMIntInt& other)
{
    m_dominatedSolutions.push_back(other);
}
