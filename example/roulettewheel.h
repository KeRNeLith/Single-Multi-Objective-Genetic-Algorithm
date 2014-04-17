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

    virtual void destroy();
    virtual void copy(const RouletteWheel<T, T2, C> &other);

public:
    RouletteWheel(const int maxChromosome = -1);
    RouletteWheel(const RouletteWheel& other);

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

    // Operator Like
    RouletteWheel& add(const RouletteWheel& op);
    // Operator
    RouletteWheel& operator=(const RouletteWheel& other);
};

template<typename T, typename T2, typename C>
RouletteWheel<T, T2, C>::RouletteWheel(const int maxChromosome)
    : Population<T, T2, C>(maxChromosome == -1 ? this->m_sNbMaxChromosomes : maxChromosome)
    , m_cumulatedFitness()
    , m_selectingProba()
{
}

template<typename T, typename T2, typename C>
RouletteWheel<T, T2, C>::RouletteWheel(const RouletteWheel& other)
    : Population<T, T2, C>(other)
{
    copy(other);
}

template<typename T, typename T2, typename C>
void RouletteWheel<T, T2, C>::destroy()
{
    Population<T, T2, C>::destroy();
    m_cumulatedFitness.clear();
    m_selectingProba.clear();
}

template<typename T, typename T2, typename C>
void RouletteWheel<T, T2, C>::copy(const RouletteWheel<T, T2, C>& other)
{
    Population<T, T2, C>::copy(other);
    m_cumulatedFitness = other.m_cumulatedFitness;
    m_selectingProba = other.m_selectingProba;
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
    // Select 2 chromosomes based on their probability beteween [0, 1] to be selected
    // It's the principle of roulette wheel, because chromosome with a better fitness
    // Have larger interval of probability to be selected

    // Select a chromosome mum and a chromosome dad
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
    // Children Chromosome
    C offspring;

    std::vector<int> offspringGenes;    // Children chromosome => will be fill
    // Get parents genes
    std::vector<int> mumGenes = parents.first.getDatas();
    std::vector<int> dadGenes = parents.second.getDatas();

    // According to crossover probability it's the dad or mum that will begin to fill offspring genes
    std::uniform_real_distribution<float> distribution(0.0, 1.0);
    float probaCrossOver = distribution(generator);

    if (C::getNbGenes() == 0)
        return offspring;

    // Random number to define on which genes crossover begin
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

    // Set offspring genes
    offspring.setDatas(offspringGenes);

    return offspring;
}

template<typename T, typename T2, typename C>
std::vector<C> RouletteWheel<T, T2, C>::getBestSolution() const
{
    if (this->m_chromosomes.empty())
        throw std::runtime_error("Error in process !");

    // Because solutions are sort, the last chromosome is the best
    std::vector< C > bestSolution;
    bestSolution.push_back(this->m_chromosomes[this->m_chromosomes.size()-1]);
    return bestSolution;
}

template<typename T, typename T2, typename C>
void RouletteWheel<T, T2, C>::addKeptChromosomes(std::vector< C > chromosomes)
{
    // Fill the population of chromosomes until it's full with chromosomes in parameter
    unsigned int i = 0;
    while (!this->isFull() && i < chromosomes.size())
        this->m_chromosomes.push_back(chromosomes[i]);
}

template<typename T, typename T2, typename C>
std::vector< C > RouletteWheel<T, T2, C>::getKeptChromosomes()
{
    // Determine the number of chromosome that will be kept
    int nbChromosomesKeep = this->m_proportionalChromosomesKeep * this->m_sNbMaxChromosomes;

    // Create a vector with the previous number of chromosome
    // Those which have the best fitness (so begin to the end of vector)
    std::vector< C > chromosomesKept;
    for (unsigned int i = 0 ; i < nbChromosomesKeep ; i++)
        chromosomesKept.push_back(this->m_chromosomes[this->m_chromosomes.size()-(1+i)]);
    return chromosomesKept;
}

template<typename T, typename T2, typename C>
RouletteWheel<T, T2, C>& RouletteWheel<T, T2, C>::add(const RouletteWheel<T, T2, C>& op)
{
    Population<T, T2, C>::add(op);

    this->m_cumulatedFitness.reserve(this->m_cumulatedFitness.size() + op.m_cumulatedFitness.size());
    this->m_selectingProba.reserve(this->m_selectingProba.size() + op.m_selectingProba.size());
    this->m_cumulatedFitness.insert(this->m_cumulatedFitness.end(), op.m_cumulatedFitness.begin(), op.m_cumulatedFitness.end());
    this->m_selectingProba.insert(this->m_selectingProba.end(), op.m_selectingProba.begin(), op.m_selectingProba.end());

    evaluateFitness();

    return *this;
}

template<typename T, typename T2, typename C>
RouletteWheel<T, T2, C>& RouletteWheel<T, T2, C>::operator=(const RouletteWheel<T, T2, C>& other)
{
    if(this != dynamic_cast<const RouletteWheel<T, T2, C>*>(&other))
    {
        destroy();
        copy(other);
    }

    return *this;
}

#endif // ROULETTEWHEEL_H
