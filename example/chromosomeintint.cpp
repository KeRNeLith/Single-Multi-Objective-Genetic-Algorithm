#include "chromosomeintint.h"

ChromosomeIntInt::ChromosomeIntInt()
    : Chromosome<int, int>()
{
}

bool ChromosomeIntInt::mutate()
{
    std::uniform_real_distribution<> distribution(0.0, 1.0);
    bool flag = false;

    for (unsigned int i = 0 ; i < m_datas.size() ; i++)
    {
        float rand = distribution(generator);
        float proba = Population<int, int, ChromosomeIntInt>::getMutateProbability();

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

void ChromosomeIntInt::computeFitness()
{
    if (m_fitness.size() != 0)
       m_fitness[0] = std::count(m_datas.begin(), m_datas.end(), 1);
    else
        m_fitness.push_back(std::count(m_datas.begin(), m_datas.end(), 1));
}

void ChromosomeIntInt::generateRandomChromosome()
{
    std::uniform_int_distribution<> distribution(0, 1);

    for (unsigned int i = 0 ; i < m_nbGenes ; i++)
        m_datas.push_back(distribution(generator));
}
