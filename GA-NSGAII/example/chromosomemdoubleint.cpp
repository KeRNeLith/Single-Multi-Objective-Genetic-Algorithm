#include "chromosomemdoubleint.h"

ChromosomeMDoubleInt::ChromosomeMDoubleInt()
    : Chromosome<double, int>()
    , m_crowdingDistance(0)
    , m_rank(-1)
    , m_dominatedSolutions()
    , m_nbSolutionDominatesMe(0)
{
}

void ChromosomeMDoubleInt::resetDominance()
{
    m_nbSolutionDominatesMe = 0;
    m_dominatedSolutions.clear();
}

bool ChromosomeMDoubleInt::dominates(const ChromosomeMDoubleInt& other)
{
    unsigned int nbMaxObjective = std::min(m_fitness.size(), other.m_fitness.size());

    // 2 conditions :
    // The current solution is no worse than other solution in all objectives
    for (unsigned int o = 0 ; o < nbMaxObjective ; o++)
    {
        if (m_fitness[o] <= other.m_fitness[o])
            continue;
        else
            return false;
    }

    // The current solution is strictly better than other solution in at least one objective
    for (unsigned int o = 0 ; o < nbMaxObjective ; o++)
    {
        if (m_fitness[o] < other.m_fitness[o])
            return true;
        else
            continue;
    }

    return false;
}

bool ChromosomeMDoubleInt::mutate()
{
    // Flip bit according to mutate probability
    // and indicate if there has been a mutation with a flag.
    std::uniform_real_distribution<> distribution(0.0, 1.0);
    bool flag = false;

    for (unsigned int i = 0 ; i < m_datas.size() ; i++)
    {
        float rand = distribution(generator);
        float proba = Population<int, int, ChromosomeMDoubleInt>::getMutateProbability();

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

void ChromosomeMDoubleInt::computeFitness()
{
    m_fitness.clear();

    // Problem SCH1 :
    // Minimize f1 =   x²
    // Minimize f2 = (x-2)²

    //const int forbidenValue1 = 0;
    const int minusValue = 2;

    double value = getDoubleFromBinary(m_datas);

    /*if(value == forbidenValue1)
    {
        double dvalue = value + 0.00001;
        this->m_fitness.push_back(1/(double)((dvalue-forbidenValue1)*(dvalue-forbidenValue1)));
        this->m_fitness.push_back(1/(double)((value-forbidenValue2)*(value-forbidenValue2)));
        return;
    }
    if(value == forbidenValue2)
    {
        double dvalue = value + 0.00001;
        this->m_fitness.push_back(1/(double)((value-forbidenValue1)*(value-forbidenValue1)));
        this->m_fitness.push_back(1/(double)((dvalue-forbidenValue2)*(dvalue-forbidenValue2)));
        return;
    }*/

    this->m_fitness.push_back(value*value);
    this->m_fitness.push_back((value-minusValue)*(value-minusValue));
}

void ChromosomeMDoubleInt::generateRandomChromosome()
{
    std::uniform_int_distribution<> distribution(0, 1);

    for (unsigned int i = 0 ; i < m_nbGenes ; i++)
        m_datas.push_back(distribution(generator));
}

void ChromosomeMDoubleInt::addDominatedSolution(ChromosomeMDoubleInt* other)
{
    m_dominatedSolutions.push_back(other);
}
