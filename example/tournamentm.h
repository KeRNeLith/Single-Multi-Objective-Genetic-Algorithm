#ifndef TOURNAMENTM_H
#define TOURNAMENTM_H

#include "population.h"

template<typename T, typename T2, typename C>
/**
 * @brief The RouletteWheel class Provide a selection of parents to breeding based on a Tournament and for Multi objective GA.
 */
class TournamentM
        : public Population<T, T2, C>
{
protected:

    std::vector< std::vector<C> > m_sortedChromosomes;

    virtual void destroy();
    virtual void copy(const TournamentM<T, T2, C> &other);
    /**
     * @brief selectOneChromosome Select one chromosome from m_chromosomes based on the crowded tournament.
     * @return A chromosome.
     */
    virtual C selectOneChromosome();

public:
    TournamentM(const int maxChromosome = -1);
    TournamentM(const TournamentM &other);

    virtual void evaluateFitness();
    virtual std::pair< C, C > selectChromosomesPair();
    virtual C crossOver(std::pair< C, C > parents);
    virtual std::vector< C > getBestSolution() const;

    // Operator Like
    TournamentM& add(const TournamentM& op);
    // Operator
    TournamentM& operator=(const TournamentM& other);
};

template<typename T, typename T2, typename C>
TournamentM<T, T2, C>::TournamentM(const int maxChromosome)
    : Population<T, T2, C>(maxChromosome == -1 ? this->m_sNbMaxChromosomes : maxChromosome)
{
}

template<typename T, typename T2, typename C>
TournamentM<T, T2, C>::TournamentM(const TournamentM& other)
    : Population<T, T2, C>(other)
{
    copy(other);
}

template<typename T, typename T2, typename C>
void TournamentM<T, T2, C>::destroy()
{
    Population<T, T2, C>::destroy();
}

template<typename T, typename T2, typename C>
void TournamentM<T, T2, C>::copy(const TournamentM<T, T2, C> &other)
{
    Population<T, T2, C>::copy(other);
}

template<typename T, typename T2, typename C>
C TournamentM<T, T2, C>::selectOneChromosome()
{
    int nbSolutions;
    if (this->m_nbMaxChromosomes != 0)
        nbSolutions = this->m_nbMaxChromosomes;
    else
        nbSolutions = this->m_sNbMaxChromosomes;

    std::uniform_int_distribution<> distributionPoolSize(2, nbSolutions);
    int poolSize = distributionPoolSize(generator);
    int proba[poolSize];

    std::uniform_int_distribution<> distribution(0, nbSolutions);

    int counter = 0;
    //double max_value = 0.0;
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

template<typename T, typename T2, typename C>
void TournamentM<T, T2, C>::evaluateFitness()
{
    for (unsigned int i = 0 ; i < this->m_chromosomes.size() ; i++)
        this->m_chromosomes[i].computeFitness();

    if (this->m_chromosomes.empty())
        return;

    unsigned int nbObjectives = this->m_chromosomes[0].getFitness().size();
    m_sortedChromosomes.clear();
    Ascending<C> comparator;    // Comaparator using to sort on ascending order each objectives
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

template<typename T, typename T2, typename C>
std::pair< C, C > TournamentM<T, T2, C>::selectChromosomesPair()
{
    return std::pair< C, C >(selectOneChromosome(), selectOneChromosome());
}

template<typename T, typename T2, typename C>
C TournamentM<T, T2, C>::crossOver(std::pair< C, C > parents)
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
std::vector< C > TournamentM<T, T2, C>::getBestSolution() const
{
    return std::vector < C >();
}

template<typename T, typename T2, typename C>
TournamentM<T, T2, C>& TournamentM<T, T2, C>::add(const TournamentM& op)
{
    Population<T, T2, C>::add(op);

    evaluateFitness();

    return *this;
}

template<typename T, typename T2, typename C>
TournamentM<T, T2, C>& TournamentM<T, T2, C>::operator=(const TournamentM& other)
{
    if(this != dynamic_cast<const TournamentM<T, T2, C>*>(&other))
    {
        destroy();
        copy(other);
    }

    return *this;
}

#endif // TOURNAMENTM_H
