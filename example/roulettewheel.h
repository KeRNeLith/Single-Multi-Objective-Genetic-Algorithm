#ifndef ROULETTEWHEEL_H
#define ROULETTEWHEEL_H

#include <string>

#include "population.h"
#include "chromosomeintint.h"

template<typename T, typename T2, typename C>
/**
 * @brief The RouletteWheel class Provide a selection of parents to breeding based on a roulette wheel.
 */
class RouletteWheel
        : public Population<T, T2, C>
{
protected:
    std::vector< T > m_cumulatedFitness;        ///> Store the cumulated fitness
    std::vector< double > m_selectingProba;     ///> Store all probability to be selected by the roulette wheel

public:
    RouletteWheel();

    virtual void evaluateFitness();
    virtual std::pair< C, C > selectChromosomesPair();
    virtual C crossOver(std::pair< C, C > parents);
    virtual std::vector< C > getBestSolution() const;

    /**
     * @brief addKeptChromosomes Add Chromosomes that must be kept between two generations.
     * @param chromosomes Vector of chromosomes that will be insert in m_chromosomes.
     */
    void addKeptChromosomes(std::vector< C > chromosomes);

    /**
     * @brief getKeptChromosomes Compute the vector of chromosomes that must be kept between each generation.
     * @return Vector of chromosomes that must be kept between each generation.
     */
    std::vector< C > getKeptChromosomes();
};

template<typename T, typename T2, typename C>
RouletteWheel<T, T2, C>::RouletteWheel()
    : Population<int, int, ChromosomeIntInt>()
    , m_cumulatedFitness()
    , m_selectingProba()
{
}

template<typename T, typename T2, typename C>
void RouletteWheel<T, T2, C>::evaluateFitness()
{
    for (unsigned int i = 0 ; i < this->m_chromosomes.size() ; i++)
        this->m_chromosomes[i].computeFitness();

    // Sort chromosomes to have m_chromosomes[0] with the lower fitness
    // and m_chromosomes[m_chromosomes.size()] with the hightest
    std::sort(this->m_chromosomes.begin(), this->m_chromosomes.end(), smallerToHigher<C>);


    if (this->m_chromosomes.empty())
        return;

    // Compute the cumulated fitness
    this->m_cumulatedFitness.clear();
    //m_cumulatedFitness.reserve(m_chromosomes.size());
    this->m_cumulatedFitness.push_back(this->m_chromosomes[0].getFitness()[0]);
    for (unsigned int i = 1 ; i < this->m_chromosomes.size() ; i++)
        this->m_cumulatedFitness.push_back(this->m_cumulatedFitness[i-1] + this->m_chromosomes[i].getFitness()[0]);

    // Compute probability to be selected by the roulette wheel
    this->m_selectingProba.clear();
    T maxFitness = this->m_cumulatedFitness[this->m_chromosomes.size()-1];
    for (unsigned int i = 0 ; i < this->m_chromosomes.size() ; i++)
         this->m_selectingProba.push_back(this->m_cumulatedFitness[i] / (double)maxFitness);
}

template<typename T, typename T2, typename C>
std::pair<C, C> RouletteWheel<T, T2, C>::selectChromosomesPair()
{
    std::uniform_real_distribution<float> distribution(0.0, 1.0);
    float probaMum = distribution(generator);
    float probaDad = distribution(generator);
    unsigned int indexMum, indexDad;

    // Find index of the mother
    for (unsigned int i = 0 ; i < this->m_chromosomes.size() ; i++)
    {
        if (probaMum > this->m_selectingProba[i])
            continue;
        else
        {
            indexMum = i;
            break;
        }
    }

    // Find index of the father
    for (unsigned int i = 0 ; i < this->m_chromosomes.size() ; i++)
    {
        if (probaDad > this->m_selectingProba[i])
            continue;
        else
        {
            indexDad = i;
            break;
        }
    }

    return std::pair< C, C >(this->m_chromosomes[indexMum], this->m_chromosomes[indexDad]);
}

template<typename T, typename T2, typename C>
C RouletteWheel<T, T2, C>::crossOver(std::pair<C, C> parents)
{
    ChromosomeIntInt offspring;

    std::vector<int> offspringGenes;
    std::vector<int> mumGenes = parents.first.getDatas();
    std::vector<int> dadGenes = parents.second.getDatas();


    std::uniform_real_distribution<float> distribution(0.0, 1.0);
    float probaCrossOver = distribution(generator);

    if (C::getNbGenes() == 0)
        return offspring;

    std::uniform_int_distribution<> distributionInt(0, C::getNbGenes()-1);
    int indexCrossover = distributionInt(generator);

    if (probaCrossOver <= this->m_crossOverProbability)
    {
        for (unsigned int i = 0 ; i <= indexCrossover ; i++)
            offspringGenes.push_back(mumGenes[i]);

        for (unsigned int i = indexCrossover + 1 ; i < C::getNbGenes()-1 ; i++)
            offspringGenes.push_back(dadGenes[i]);
    }
    else
    {
        for (unsigned int i = 0 ; i <= indexCrossover ; i++)
            offspringGenes.push_back(dadGenes[i]);

        for (unsigned int i = indexCrossover + 1 ; i < C::getNbGenes()-1 ; i++)
            offspringGenes.push_back(mumGenes[i]);
    }

    offspring.setDatas(offspringGenes);

    return offspring;
}

template<typename T, typename T2, typename C>
std::vector<C> RouletteWheel<T, T2, C>::getBestSolution() const
{
    if (this->m_chromosomes.empty())
        throw std::runtime_error("Error in process !");

    std::vector< C > bestSolution;
    bestSolution.push_back(this->m_chromosomes[this->m_chromosomes.size()-1]);
    return bestSolution;
}

template<typename T, typename T2, typename C>
void RouletteWheel<T, T2, C>::addKeptChromosomes(std::vector< C > chromosomes)
{
    unsigned int i = 0;
    while (!this->isFull() && i < chromosomes.size())
        this->m_chromosomes.push_back(chromosomes[i]);
}

template<typename T, typename T2, typename C>
std::vector< C > RouletteWheel<T, T2, C>::getKeptChromosomes()
{
    int nbChromosomesKeep = this->m_proportionalChromosomesKeep * this->m_nbMaxChromosomes;

    std::vector< C > chromosomesKept;
    for (unsigned int i = 0 ; i < nbChromosomesKeep ; i++)
        chromosomesKept.push_back(this->m_chromosomes[this->m_chromosomes.size()-(1+i)]);
    return chromosomesKept;
}

#endif // ROULETTEWHEEL_H
