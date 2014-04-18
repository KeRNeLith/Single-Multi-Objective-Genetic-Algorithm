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
    unsigned int nbMaxObjective = std::min(m_fitness.size(), other.m_fitness.size());

    // 2 conditions :
    // The current solution is no worse than other solution in all objectives
    for (unsigned int o = 0 ; o < nbMaxObjective ; o++)
    {
        if (m_fitness[o] >= other.m_fitness[o])
            continue;
        else
            return false;
    }

    // The current solution is strictly better than other solution in at least one objective
    for (unsigned int o = 0 ; o < nbMaxObjective ; o++)
    {
        if (m_fitness[o] > other.m_fitness[o])
            return true;
        else
            continue;
    }

    return false;
}

bool ChromosomeMIntInt::mutate()
{
    // Flip bit according to mutate probability
    // and indicate if there has been a mutation with a flag.
    std::uniform_real_distribution<> distribution(0.0, 1.0);
    bool flag = false;

    for (unsigned int i = 0 ; i < m_datas.size() ; i++)
    {
        float rand = distribution(generator);
        float proba = Population<int, int, ChromosomeMIntInt>::getMutateProbability();

        if (rand > proba)
            continue;
        if (m_datas[i] == 0)
            m_datas[i] = 1;
        else
            m_datas[i] = 0;
        flag = true;
    }

    return flag;
}

void ChromosomeMIntInt::computeFitness()
{

}

void ChromosomeMIntInt::generateRandomChromosome()
{
    std::uniform_int_distribution<> distribution(0, 1);

    for (unsigned int i = 0 ; i < m_nbGenes ; i++)
        m_datas.push_back(distribution(generator));
}

void ChromosomeMIntInt::addDominatedSolution(const ChromosomeMIntInt& other)
{
    m_dominatedSolutions.push_back(other);
}
