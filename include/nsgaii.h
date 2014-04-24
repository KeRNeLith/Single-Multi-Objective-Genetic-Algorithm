#ifndef NSGAII_H
#define NSGAII_H

#include <iostream>

#include "ga.h"

template<typename T, typename P, typename C>
class NSGAII
        : public GA<T, P, C>
{
protected:

    P* m_offspring; ///> Offspring of m_population.

    virtual void releaseMemory();

    /**
     * @brief runOneGeneration Do all steps needed for one generation (iteration), like mutation and crossover operators, etc...
     */
    virtual void runOneGeneration();

    /**
     * @brief breeding Perform GA operators (selection, crossover, mutation) on m_population.
     * @return The offspring population.
     */
    virtual P* breeding();

    /**
     * @brief fastNonDominatedSort Determine all non-dominated front of the Population in parameter.
     * @param popToSort Population to search non-dominated fronts.
     * @return Vector of non-dominated fronts, fisrt element correspond to front 1 and so on.
     */
    virtual std::vector < P > fastNonDominatedSort(P* popToSort);

    /**
     * @brief crowdingDistanceAssignement Compute the crowding distance on the popualtion in parameter.
     * @param popToAssignCrowdingDistance Population which we want to compute crowding distance.
     */
    virtual void crowdingDistanceAssignement(P* popToAssignCrowdingDistance);

    /**
     * @brief addChromosomeWithoutControl Add a Chromosome chromosome to the population pop, if pop will be full, it add the chromosome and increment max Population size by one.
     * @param pop Population that we want to add Chromosome.
     * @param chromosome Chromosome to add.
     */
    virtual void addChromosomeWithoutControl(P* pop, C* chromosome);

    /**
     * @brief displayAdvancement Display in the console advancement of the algorithm.
     */
    virtual void displayAdvancement();

public:
    NSGAII();
    virtual ~NSGAII();

    virtual void initialize();
    virtual std::vector< C > performGA();
    virtual void reset();
};

template<typename T, typename P, typename C>
NSGAII<T, P, C>::NSGAII()
    : GA<T, P, C>()
    , m_offspring(nullptr)
{
}

template<typename T, typename P, typename C>
NSGAII<T, P, C>::~NSGAII()
{
    releaseMemory();
}

template<typename T, typename P, typename C>
void NSGAII<T, P, C>::releaseMemory()
{
    GA<T, P, C>::releaseMemory();
    if (m_offspring)
        delete m_offspring;
    m_offspring = nullptr;
}

template<typename T, typename P, typename C>
void NSGAII<T, P, C>::runOneGeneration()
{
    // Combine parent and offsprings population
    this->m_population->add(*m_offspring);

    // Determine all non dominated fronts
    std::vector < P > fronts = fastNonDominatedSort(this->m_population);

    // Secure check if it's not empty (should never arrived)
    if (fronts.empty())
    {
        this->m_currentGeneration++;
        return;
    }

    P* newParents = new P;  // Future population
    int i = 0;              // Index front
    // Until the population is filled
    while (newParents->getCurrentNbChromosomes() + fronts[i].getCurrentNbChromosomes() <= newParents->getNbMaxChromosomes())
    {
        // Calculate crowding-distance in ith Front
        crowdingDistanceAssignement(&fronts[i]);
        // Include ith non-dominated front in the population
        newParents->addChromosomes(fronts[i].getChromosomes());
        i++;    // Check the next front for inclusion
    }

    if (!newParents->isFull())
    {
        // Calculate crowding-distance in ith Front, because previous loop stop just before calculate these values
        crowdingDistanceAssignement(&fronts[i]);
        std::vector< C > chromosomes = fronts[i].getChromosomes();

        // Sort in descending order the ith front using crowding operator
        std::sort(chromosomes.begin(), chromosomes.end(), crowdingOperator<C>);

        // Choose the first (max chromosomes - size newParents) of ith front
        newParents->addChromosomes(fronts[i].getChromosomes(), newParents->getNbMaxChromosomes() - newParents->getCurrentNbChromosomes());
    }

    delete this->m_population;
    this->m_population = new P;
    *this->m_population = *newParents;
    // Use selection, crossover and mutation to create new offspring population
    delete m_offspring;
    // Execute breeding (create offspring using GA)
    m_offspring = breeding();

    // Only for Display
    displayAdvancement();

    this->m_currentGeneration++;    // Generation counter
}

template<typename T, typename P, typename C>
P* NSGAII<T, P, C>::breeding()
{
    P* newPop = new P;

    while (!newPop->isFull())
    {
        C chromosome = this->m_population->crossOver(this->m_population->selectChromosomesPair());
        newPop->addChromosome(chromosome);
    }

    newPop->mutate();
    newPop->evaluateFitness();

    return newPop;
}

template<typename T, typename P, typename C>
std::vector<P> NSGAII<T, P, C>::fastNonDominatedSort(P* popToSort)
{
    // Will contain all fronts
    std::vector< P > fronts;

    // Recover population's vector of chomosomes
    std::vector< C > chromosomes = popToSort->getChromosomes();
    P* front1 = new P;
    // Determine first front
    for (unsigned int p = 0 ; p < chromosomes.size() ; p++)
    {
        // Sp = void | np = 0
        chromosomes[p].resetDominance();
        for (unsigned int q = 0 ; q < chromosomes.size() ; q++ )
        {
            if (chromosomes[p] == chromosomes[q]) // case when we compare same chromosomes
                continue;

            if (chromosomes[p].dominates(chromosomes[q]))       // p dominates q
                chromosomes[p].addDominatedSolution(&chromosomes[q]);    // Sp = Sp U {p}
            else if (chromosomes[q].dominates(chromosomes[p]))  // q dominates p
                chromosomes[p].setNbSolutionDominatesMe(chromosomes[p].getNbSolutionDominatesMe()+1);   // np + 1
        }

        // p belongs to the first front
        if (chromosomes[p].getNbSolutionDominatesMe() == 0)
        {
            chromosomes[p].setRank(0);
            addChromosomeWithoutControl(front1, &chromosomes[p]);
        }
    }
    fronts.push_back(*front1);
    delete front1;
    // Reaffect chromosomes to the population to sort
    popToSort->setChromosomes(chromosomes);

    // Determine other fronts
    int i = 0; // Initialize front counter
    P* Q = new P;
    while (fronts[i].getCurrentNbChromosomes() != 0)
    {
        Q->reset();
        for (unsigned int p = 0 ; p < fronts[i].getChromosomes().size() ; p++)
        {
            for (unsigned int q = 0 ; q < fronts[i].getChromosomes()[p].getDominatedSolution().size() ; q++)
            {
                int nq = fronts[i].getChromosomes()[p].getDominatedSolution()[q]->getNbSolutionDominatesMe();
                fronts[i].getChromosomes()[p].getDominatedSolution()[q]->setNbSolutionDominatesMe(nq-1);
                if (fronts[i].getChromosomes()[p].getDominatedSolution()[q]->getNbSolutionDominatesMe() == 0)    // q belongs to the next front, nq = 0
                {
                    fronts[i].getChromosomes()[p].getDominatedSolution()[q]->setRank(i+1);   // qrank = i + 1
                    addChromosomeWithoutControl(Q, fronts[i].getChromosomes()[p].getDominatedSolution()[q]);    // Q = Q U {q}
                }
            }
        }
        i++;
        fronts.push_back(*Q);
    }
    // Delete fronts created if it is empty.
    auto removeEnd = std::remove_if(fronts.begin(), fronts.end(), emptyPopulation<P>);
    if (removeEnd != fronts.end())
        fronts.erase(removeEnd, fronts.end());

    return fronts;
}

template<typename T, typename P, typename C>
void NSGAII<T, P, C>::crowdingDistanceAssignement(P* popToAssignCrowdingDistance)
{
    if (popToAssignCrowdingDistance->getChromosomes().empty())
        return;

    // Number of solutions in popToAssignCrowdingDistance
    unsigned int nbSolutions = popToAssignCrowdingDistance->getCurrentNbChromosomes();

    // Initialize distance
    for (unsigned int i = 0 ; i < nbSolutions ; i++)
        popToAssignCrowdingDistance->getChromosomes()[i].setDistance(0);

    Ascending<C> comparator;    // Comparator using to sort on ascending order each objectives
    unsigned int nbObjective = popToAssignCrowdingDistance->getChromosomes()[0].getNbObjective();
    for (unsigned int m = 0 ; m < nbObjective ; m++)
    {
        std::vector< C > chromosomes = popToAssignCrowdingDistance->getChromosomes();

        // Sort using each objective value
        comparator.index = m;
        std::sort(chromosomes.begin(),
                  chromosomes.end(),
                  comparator);

        // Assigne value of max minus min of fitness for the objective m
        T maxMinusMinFitness = chromosomes[nbSolutions-1].getFitness()[m] - chromosomes[0].getFitness()[m];

        if (maxMinusMinFitness == 0) // Little cheat to prevent division by 0
            maxMinusMinFitness = 0.000000001;
        if (chromosomes.size() == 1) // Little cheat to prevent out of range access
        {
            double distance =   chromosomes[0].getDistance()
                                + (chromosomes[0].getFitness()[m] - chromosomes[0].getFitness()[m])
                                / (double)(maxMinusMinFitness);

            chromosomes[0].setDistance(distance);

            // Reaffect chromosomes to the population
            popToAssignCrowdingDistance->setChromosomes(chromosomes);
            continue;
        }

        // So that boundary point are always selected
        // Extremes chromosomes of the vector are initialized with an infinite distance
        chromosomes[0].setDistance(std::numeric_limits<double>::max());
        chromosomes[nbSolutions-1].setDistance(std::numeric_limits<double>::max());

        // For all other points
        for (unsigned int i = 1 ; i < nbSolutions-2 ; i++)
        {
            double distance =   chromosomes[i].getDistance()
                                + (chromosomes[i+1].getFitness()[m] - chromosomes[i-1].getFitness()[m])
                                / (double)(maxMinusMinFitness);

            chromosomes[i].setDistance(distance);
        }

        // Reaffect chromosomes to the population
        popToAssignCrowdingDistance->setChromosomes(chromosomes);
    }
}

template<typename T, typename P, typename C>
void NSGAII<T, P, C>::addChromosomeWithoutControl(P* pop, C* chromosome)
{
    if (pop->isFull())
        pop->setNbMaxChromosomes(pop->getNbMaxChromosomes()+1);
    pop->addChromosome(*chromosome);
}

template<typename T, typename P, typename C>
void NSGAII<T, P, C>::displayAdvancement()
{
    const int nbSymbols = 40;
    double advancement = this->m_currentGeneration / (double)this->m_nbGenerationsWanted;
    double nbSymbolsToDraw = nbSymbols * advancement;
    std::cout << "\r" << "[";
    for (int i = 0 ; i < nbSymbols ; i++)
    {
        if (i <= nbSymbolsToDraw)
            std::cout << "=";
        else
            std::cout << " ";
    }
    std::cout << "]" << "\t" << advancement*100 << "%";
}

template<typename T, typename P, typename C>
void NSGAII<T, P, C>::initialize()
{
    if(this->m_isInitialized)   // already initialized
        return;

    this->releaseMemory();

    // Generate a random population make step that need to be done before running algorithm
    this->m_population = new P;
    this->m_population->generateRandomChromosomes();
    this->m_population->evaluateFitness();

    // Create offspring of the random population
    this->m_offspring = breeding();
    this->m_offspring->evaluateFitness();

    this->m_currentGeneration = 1;
    this->m_isInitialized = true;
}

template<typename T, typename P, typename C>
std::vector< C > NSGAII<T, P, C>::performGA()
{
    // Run the algorithm if it is initialized, and return Pareto Optimal Set
    if (!this->m_isInitialized)
        throw std::runtime_error("GA not initialzed !");

    while (this->m_currentGeneration <= this->m_nbGenerationsWanted)
        runOneGeneration();

    // TODO
    //return this->m_population->getBestSolution();
    return std::vector < C >();
}

template<typename T, typename P, typename C>
void NSGAII<T, P, C>::reset()
{
    this->m_currentGeneration = 1;
    this->m_isInitialized = false;
}

#endif // NSGAII_H
