#ifndef TOURNAMENTM_H
#define TOURNAMENTM_H

#include "population.h"

template<typename F, typename DATA, typename C>
/**
 * @brief The RouletteWheel class Provide a selection of parents to breeding based on a Tournament and for Multi objective GA.
 */
class TournamentM
        : public Population<F, DATA, C>
{
protected:

    std::vector< std::vector< C > > m_sortedChromosomes;

    virtual void destroy();
    virtual void copy(const TournamentM<F, DATA, C> &other);

public:
    TournamentM(const int maxChromosome = -1);
    TournamentM(const TournamentM &other);

    virtual void evaluateFitness();
    virtual C selectOneChromosome();
    virtual std::pair< C, C > selectChromosomesPair();
    virtual C crossOver(std::pair< C, C > parents);
    virtual std::vector< C > getBestSolution() const;

    // Operator Like
    TournamentM& add(const TournamentM& op);
    // Operator
    TournamentM& operator=(const TournamentM& other);
};

template<typename F, typename DATA, typename C>
TournamentM<F, DATA, C>::TournamentM(const int maxChromosome)
    : Population<F, DATA, C>(maxChromosome == -1 ? this->m_sNbMaxChromosomes : maxChromosome)
{
}

template<typename F, typename DATA, typename C>
TournamentM<F, DATA, C>::TournamentM(const TournamentM& other)
    : Population<F, DATA, C>(other)
{
    copy(other);
}

template<typename F, typename DATA, typename C>
void TournamentM<F, DATA, C>::destroy()
{
    Population<F, DATA, C>::destroy();
}

template<typename F, typename DATA, typename C>
void TournamentM<F, DATA, C>::copy(const TournamentM<F, DATA, C> &other)
{
    Population<F, DATA, C>::copy(other);
}

template<typename F, typename DATA, typename C>
C TournamentM<F, DATA, C>::selectOneChromosome()
{
    std::uniform_int_distribution<> distributionPoolSize(2, this->m_nbMaxChromosomes);
    int poolSize = distributionPoolSize(generator);
    int proba[poolSize];

    std::uniform_int_distribution<> distribution(0, this->m_nbMaxChromosomes-1);

    int counter = 0;
    bool equalIndex = false;
    do{
        // Generate random Index (different of of each other index already selected)
        do{
            equalIndex = false;
            proba[counter] = distribution(generator);
            for (int i = 0; i < counter; i++)
            {
                if( proba[counter] == proba[i] )
                {
                    equalIndex = true;
                    break;
                }
            }
        } while( equalIndex );

        counter++;

    } while(counter != poolSize);

    C chromosome = this->m_chromosomes[proba[0]];
    for (int i = 0 ; i < poolSize ; i++)
    {
        if( this->m_chromosomes[proba[i]].getRank() < chromosome.getRank() // rank i < rank chromosome
            || (this->m_chromosomes[proba[i]].getRank() == chromosome.getRank() && this->m_chromosomes[proba[i]].getDistance() > chromosome.getDistance())) // rank i == rank chromosome && dist i > dist chromosome
            chromosome = this->m_chromosomes[proba[i]];
    }

    return chromosome;
}

template<typename F, typename DATA, typename C>
void TournamentM<F, DATA, C>::evaluateFitness()
{
    for (unsigned int i = 0 ; i < this->m_chromosomes.size() ; i++)
        this->m_chromosomes[i].computeFitness();

    if (this->m_chromosomes.empty())
        return;

    unsigned int nbObjectives = this->m_chromosomes[0].getFitness().size();
    m_sortedChromosomes.clear();
    Ascending< C > comparator;    // Comaparator using to sort on ascending order each objectives
    // Sort chromosomes to have m_chromosomes[0] with the lower fitness
    // and m_chromosomes[m_chromosomes.size()] with the hightest
    // On each objectives
    for (unsigned int o = 0 ; o < nbObjectives ; o++)
    {
        comparator.index = o;
        m_sortedChromosomes.push_back(this->m_chromosomes);
        std::sort(m_sortedChromosomes[o].begin(), m_sortedChromosomes[o].end(), comparator);
    }
}

template<typename F, typename DATA, typename C>
std::pair< C, C > TournamentM<F, DATA, C>::selectChromosomesPair()
{
    return std::pair< C, C >(selectOneChromosome(), selectOneChromosome());
}

template<typename F, typename DATA, typename C>
C TournamentM<F, DATA, C>::crossOver(std::pair< C, C > parents)
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

    if (probaCrossOver <= 0.5)  // Take mum genes before dad, with 1/2
    {
        for (unsigned int i = 0 ; i < indexCrossover ; i++)
            offspringGenes.push_back(mumGenes[i]);

        for (unsigned int i = indexCrossover ; i < C::getNbGenes() ; i++)
            offspringGenes.push_back(dadGenes[i]);
    }
    else    // Take dad genes before mum
    {
        for (unsigned int i = 0 ; i < indexCrossover ; i++)
            offspringGenes.push_back(dadGenes[i]);

        for (unsigned int i = indexCrossover ; i < C::getNbGenes() ; i++)
            offspringGenes.push_back(mumGenes[i]);
    }

    // Set offspring genes
    offspring.setDatas(offspringGenes);

    return offspring;
}

template<typename F, typename DATA, typename C>
std::vector< C > TournamentM<F, DATA, C>::getBestSolution() const
{
    return this->m_chromosomes;
}

template<typename F, typename DATA, typename C>
TournamentM<F, DATA, C>& TournamentM<F, DATA, C>::add(const TournamentM& op)
{
    Population<F, DATA, C>::add(op);

    evaluateFitness();

    return *this;
}

template<typename F, typename DATA, typename C>
TournamentM<F, DATA, C>& TournamentM<F, DATA, C>::operator=(const TournamentM& other)
{
    if(this != dynamic_cast<const TournamentM<F, DATA, C>*>(&other))
    {
        destroy();
        copy(other);
    }

    return *this;
}

#endif // TOURNAMENTM_H
