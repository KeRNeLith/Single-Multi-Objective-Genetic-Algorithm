#ifndef ROULETTEWHEEL_H
#define ROULETTEWHEEL_H

#include <string>

#include "population.h"
#include "chromosomeintint.h"

template<typename C>
class RouletteWheel
        : public Population<int, int, ChromosomeIntInt>
{
protected:
    std::vector< int > m_cumulatedFitness;    ///> Store the cumulated fitness
    std::vector< double > m_selectingProba;      ///> Store all probability to be selected by the roulette wheel

public:
    RouletteWheel();
    virtual ~RouletteWheel();

    virtual void evaluateFitness();
    virtual std::pair< C, C > selectChromosomesPair();
    virtual C crossOver(std::pair< C, C > parents);
};

template<typename C>
RouletteWheel<C>::RouletteWheel()
    : Population<int, int, ChromosomeIntInt>()
    , m_cumulatedFitness()
    , m_selectingProba()
{
}

template<typename C>
RouletteWheel<C>::~RouletteWheel()
{
}

template<typename C>
void RouletteWheel<C>::evaluateFitness()
{
    Population<int, int, ChromosomeIntInt>::evaluateFitness();

    if (m_chromosomes.empty())
        return;

    // Compute the cumulated fitness
    m_cumulatedFitness.clear();
    //m_cumulatedFitness.reserve(m_chromosomes.size());
    m_cumulatedFitness.push_back(m_chromosomes[0].getFitness());
    for (unsigned int i = 1 ; i < m_chromosomes.size() ; i++)
        m_cumulatedFitness.push_back(m_cumulatedFitness[i-1] + m_chromosomes[i].getFitness());

    // Compute probability to be selected by the roulette wheel
    m_selectingProba.clear();
    int maxFitness = m_cumulatedFitness[m_chromosomes.size()-1];
    for (unsigned int i = 0 ; i < m_chromosomes.size() ; i++)
         m_selectingProba.push_back(m_cumulatedFitness[i] / (double)maxFitness);
}

template<typename C>
std::pair<C, C> RouletteWheel<C>::selectChromosomesPair()
{
    std::uniform_real_distribution<float> distribution(0.0, 1.0);
    float probaMum = distribution(generator);
    float probaDad = distribution(generator);
    unsigned int indexMum, indexDad;

    // Find index of the mother
    for (unsigned int i = 0 ; i < m_chromosomes.size() ; i++)
    {
        if (probaMum > m_selectingProba[i])
            continue;
        else
        {
            indexMum = i;
            break;
        }
    }

    // Find index of the father
    for (unsigned int i = 0 ; i < m_chromosomes.size() ; i++)
    {
        if (probaDad > m_selectingProba[i])
            continue;
        else
        {
            indexDad = i;
            break;
        }
    }

    return std::pair< C, C >(m_chromosomes[indexMum], m_chromosomes[indexDad]);
}

template<typename C>
C RouletteWheel<C>::crossOver(std::pair<C, C> parents)
{
    ChromosomeIntInt offspring;

    std::vector<int> offspringGenes;
    std::vector<int> mumGenes = parents.first.getDatas();
    std::vector<int> dadGenes = parents.second.getDatas();


    std::uniform_real_distribution<float> distribution(0.0, 1.0);
    float probaCrossOver = distribution(generator);

    if (ChromosomeIntInt::getNbGenes() == 0)
        return offspring;

    std::uniform_int_distribution<> distributionInt(0, ChromosomeIntInt::getNbGenes()-1);
    int indexCrossover = distributionInt(generator);

    if (probaCrossOver <= m_crossOverProbability)
    {
        for (unsigned int i = 0 ; i <= indexCrossover ; i++)
            offspringGenes.push_back(mumGenes[i]);

        for (unsigned int i = indexCrossover + 1 ; i < ChromosomeIntInt::getNbGenes()-1 ; i++)
            offspringGenes.push_back(dadGenes[i]);
    }
    else
    {
        for (unsigned int i = 0 ; i <= indexCrossover ; i++)
            offspringGenes.push_back(dadGenes[i]);

        for (unsigned int i = indexCrossover + 1 ; i < ChromosomeIntInt::getNbGenes()-1 ; i++)
            offspringGenes.push_back(mumGenes[i]);
    }

    offspring.setDatas(offspringGenes);

    return offspring;
}

#endif // ROULETTEWHEEL_H
