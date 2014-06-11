#ifndef TOURNAMENTM_H
#define TOURNAMENTM_H

#include <future>

#include "population.h"

template<typename F, typename DATA, typename C>
/**
 * @brief The TournamentM class Provide a selection of parents to breeding based on a Tournament and for Multi objective GA.
 */
class TournamentM
        : public smoga::Population<F, DATA, C>
{
protected:

    virtual void destroy();
    virtual void copy(const TournamentM<F, DATA, C> &other);
    void computeFitnessForTheRange(const unsigned int begin, const unsigned int end);

public:
    TournamentM(const int maxChromosome = -1);
    TournamentM(const TournamentM &other);

    virtual void evaluateFitness();
    virtual C selectOneChromosome();
    virtual std::pair< C, C > selectChromosomesPair();
    virtual C crossOver(const std::pair<C, C> parents);
    virtual std::vector< C > getBestSolution() const;

    // Operator Like
    TournamentM& add(const TournamentM& op);
    // Operator
    TournamentM& operator=(const TournamentM& other);
};

template<typename F, typename DATA, typename C>
TournamentM<F, DATA, C>::TournamentM(const int maxChromosome)
    : smoga::Population<F, DATA, C>(maxChromosome == -1 ? this->m_sNbMaxChromosomes : maxChromosome)
{
}

template<typename F, typename DATA, typename C>
TournamentM<F, DATA, C>::TournamentM(const TournamentM& other)
    : smoga::Population<F, DATA, C>(other)
{
    copy(other);
}

template<typename F, typename DATA, typename C>
void TournamentM<F, DATA, C>::destroy()
{
    smoga::Population<F, DATA, C>::destroy();
}

template<typename F, typename DATA, typename C>
void TournamentM<F, DATA, C>::copy(const TournamentM<F, DATA, C> &other)
{
    smoga::Population<F, DATA, C>::copy(other);
}

template<typename F, typename DATA, typename C>
void TournamentM<F, DATA, C>::computeFitnessForTheRange(const unsigned int begin, const unsigned int end)
{
    for (unsigned int i = begin ; i <= end ; ++i)
        this->m_chromosomes[i].computeFitness();
}

template<typename F, typename DATA, typename C>
C TournamentM<F, DATA, C>::selectOneChromosome()
{
    std::uniform_int_distribution<> distributionPoolSize(2, this->m_nbMaxChromosomes);
    const unsigned int poolSize = distributionPoolSize(generator);
    int proba[poolSize];

    std::uniform_int_distribution<> distribution(0, this->m_nbMaxChromosomes-1);

    int counter = 0;
    bool equalIndex = false;
    do{
        // Generate random Index (different of of each other index already selected)
        do{
            equalIndex = false;
            proba[counter] = distribution(generator);
            for (int i = 0; i < counter; ++i)
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
    for (unsigned int i = 0 ; i < poolSize ; ++i)
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
    if (this->m_chromosomes.empty())
        return;

    const unsigned int nbChromosomes = this->m_chromosomes.size();
    /*const unsigned int maxNbThread = std::thread::hardware_concurrency();
    if (maxNbThread > 2)    // Parallelize only if there are more than 2 cores
    {
        const unsigned int nbComputePerAsync = nbChromosomes/maxNbThread;

        unsigned int begin = 0;
        unsigned int end;
        std::vector< std::future<void> > computeFitnessPartFunc;
        for (unsigned int i = 0 ; i < maxNbThread ; ++i)
        {
            end = begin + nbComputePerAsync - 1;
            if (i == maxNbThread-1)
                end = nbChromosomes-1;

            computeFitnessPartFunc.push_back(std::async(std::launch::async,
                                                        [this](const unsigned int begin, const unsigned int end)
                                                        {
                                                            return this->computeFitnessForTheRange(begin, end);
                                                        },
                                                        begin,
                                                        end));

            begin += nbComputePerAsync;
        }

        for (unsigned int i = 0 ; i < maxNbThread ; ++i)
            computeFitnessPartFunc[i].get();
    }
    else
    {*/
        for (unsigned int i = 0 ; i < nbChromosomes ; ++i)
            this->m_chromosomes[i].computeFitness();
    //}
}

template<typename F, typename DATA, typename C>
std::pair< C, C > TournamentM<F, DATA, C>::selectChromosomesPair()
{
    return std::pair< C, C >(selectOneChromosome(), selectOneChromosome());
}

template<typename F, typename DATA, typename C>
C TournamentM<F, DATA, C>::crossOver(const std::pair< C, C > parents)
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

    const unsigned int nbGenes = C::getNbGenes();
    if (nbGenes == 0)
        return offspring;

    // Random number to define on which genes crossover begin
    std::uniform_int_distribution<> distributionInt(0, C::getNbGenes()-1);
    const unsigned int indexCrossover = distributionInt(generator);

    if (probaCrossOver <= 0.5)  // Take mum genes before dad, with 1/2
    {
        for (unsigned int i = 0 ; i < indexCrossover ; ++i)
            offspringGenes.push_back(mumGenes[i]);

        for (unsigned int i = indexCrossover ; i < nbGenes ; ++i)
            offspringGenes.push_back(dadGenes[i]);
    }
    else    // Take dad genes before mum
    {
        for (unsigned int i = 0 ; i < indexCrossover ; ++i)
            offspringGenes.push_back(dadGenes[i]);

        for (unsigned int i = indexCrossover ; i < nbGenes ; ++i)
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
    smoga::Population<F, DATA, C>::add(op);

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
