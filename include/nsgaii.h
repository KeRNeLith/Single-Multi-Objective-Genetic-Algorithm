#ifndef NSGAII_H
#define NSGAII_H

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
    virtual void addChromosomeWithoutControl(P* pop, const C& chromosome);

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

#include <iostream>
template<typename T, typename P, typename C>
void NSGAII<T, P, C>::runOneGeneration()
{
    // Combine parent and offsprings population
    this->m_population->add(*m_offspring);

    // TO REMOVE
    /*for (unsigned int i = 0 ; i < this->m_population->getCurrentNbChromosomes() ; i++)
        std::cout << "chromosome " << i << " : nb objective : "<<  this->m_population->getChromosomes()[i].getFitness().size() << std::endl;
    std::cout << "Last chromosome " << " : objectives :";
    for (unsigned int i = 0 ; i < this->m_population->getChromosomes()[this->m_population->getCurrentNbChromosomes()-1].getFitness().size() ; i++)
        std::cout << " " <<  this->m_population->getChromosomes()[this->m_population->getCurrentNbChromosomes()-1].getFitness()[i];
    std::cout << std::endl;*/
    // TO REMOVE

    // Determine all non dominated fronts
    std::vector < P > fronts = fastNonDominatedSort(this->m_population);
    /*std::cout /*<< "pop size : " << this->m_population->getCurrentNbChromosomes() << std::endl
                << "pop max : " << this->m_population->getNbMaxChromosomes() << std::endl
                *//*<< "size fronts : " << fronts.size() << std::endl << std::endl;*/

    // Secure check if it's not empty (should never arrived)
    if (fronts.empty())
    {
        this->m_currentGeneration++;
        return;
    }

    P* newParents = new P;  // Future population
    int i = 0;              // Index front

    std::cout << "newParents size : " << newParents->getCurrentNbChromosomes()<<std::endl
              << "newParents max : " << newParents->getNbMaxChromosomes() <<std::endl
              << "front0 size : " << fronts[i].getCurrentNbChromosomes()<<std::endl
              << "front0 max : " << fronts[i].getNbMaxChromosomes()<<std::endl
              << "condition loop : " << (newParents->getCurrentNbChromosomes() + fronts[i].getCurrentNbChromosomes() <= newParents->getNbMaxChromosomes() ? "true" : "false") << std::endl;

    // Until the population is filled
    while (newParents->getCurrentNbChromosomes() + fronts[i].getCurrentNbChromosomes() <= newParents->getNbMaxChromosomes())
    {
        // Calculate crowding-distance in ith Front
        crowdingDistanceAssignement(&fronts[i]);
        // Include ith non-dominated front in the population
        newParents->addChromosomes(fronts[i].getChromosomes());
        i++;    // Check the next front for inclusion
    }

    // Sort in descending order the ith front using crowding operator
    std::sort(fronts[i].getChromosomes().begin(), fronts[i].getChromosomes().end(), crowdingOperator<C>);

    // Choose the first (max chromosomes - size newParents) of ith front
    delete this->m_population;
    this->m_population = new P;
    newParents->addChromosomes(fronts[i].getChromosomes(), newParents->getNbMaxChromosomes() - newParents->getCurrentNbChromosomes());
    *this->m_population = *newParents;
    // Use selection, crossover and mutation to create new offspring population
    delete m_offspring;
    // Execute breeding (create offspring using GA)
    m_offspring = breeding();

    this->m_currentGeneration++;    // Generation counter
}

template<typename T, typename P, typename C>
P* NSGAII<T, P, C>::breeding()
{
    // TODO
    P* newPop = new P;
    newPop->generateRandomChromosomes();
    /*while (!newPop->isFull())
    {
        C chromosome = this->m_population->crossOver(this->m_population->selectChromosomesPair());
        newPop->addChromosome(chromosome);
    }
    newPop->mutate();
    newPop->evaluateFitness();*/

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
                chromosomes[p].addDominatedSolution(chromosomes[q]);    // Sp = Sp U {p}
            else if (chromosomes[q].dominates(chromosomes[p]))  // q dominates p
                chromosomes[p].setNbSolutionDominatesMe(chromosomes[p].getNbSolutionDominatesMe()+1);   // np + 1
        }

        // p belongs to the first front
        if (chromosomes[p].getNbSolutionDominatesMe() == 0)
        {
            chromosomes[p].setRank(0);
            addChromosomeWithoutControl(front1, chromosomes[p]);
        }
    }
    fronts.push_back(*front1);
    delete front1;
    // Reaffect chromosomes to the population to sort
    popToSort->setChromosomes(chromosomes);



    std::cout << "Size " <<fronts[0].getCurrentNbChromosomes() << " => rank front 1 :"<< std::endl;
    for(unsigned int i = 0 ; i < fronts[0].getCurrentNbChromosomes() ; i++)
        std::cout << " " << fronts[0].getChromosomes()[i].getRank();
    std::cout <<std::endl<<std::endl<<std::endl;

    //std::cout << "'fastNonDominatedSort' : size front1 " << fronts[0].getCurrentNbChromosomes() << std::endl<< std::endl;

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
                //std::cout << "loop on front "<<i<<" chromosome "<<p<<"/" << fronts[i].getCurrentNbChromosomes()-1<<" : "<<q<<"/"<< frontiChromosomes[p].getDominatedSolution().size()-1 << std::endl;
                // nq = nq - 1
                int nq = fronts[i].getChromosomes()[p].getDominatedSolution()[q].getNbSolutionDominatesMe();
                /*std::cout << "n avant = "<< nq<<std::endl;
                nq -=1;
                std::cout << "je pige rien n vaut : "<<nq<<std::endl;*/
                fronts[i].getChromosomes()[p].getDominatedSolution()[q].setNbSolutionDominatesMe(nq-1);

                std::cout << "n = "<< fronts[i].getChromosomes()[p].getDominatedSolution()[q].getNbSolutionDominatesMe()<<std::endl;

                if (fronts[i].getChromosomes()[p].getDominatedSolution()[q].getNbSolutionDominatesMe() == 0)    // q belongs to the next front, nq = 0
                {
                    std::cout << "n ds if = "<< fronts[i].getChromosomes()[p].getDominatedSolution()[q].getNbSolutionDominatesMe()<< " solution q= " <<q<<std::endl;
                    fronts[i].getChromosomes()[p].getDominatedSolution()[q].setRank(i+1);   // qrank = i + 1
                    addChromosomeWithoutControl(Q, fronts[i].getChromosomes()[p].getDominatedSolution()[q]);    // Q = Q U {q}
                }
            }
        }
        i++;
        fronts.push_back(*Q);
        //TO REMOVE
        std::cout << "LOOP => "<<i-1<<" "<<std::endl;
        for (unsigned int k = 0 ; k < fronts.size() ; k++)
        {
            std::cout << "Size " << fronts[k].getCurrentNbChromosomes() << " => rank front " <<k<<" :"<< std::endl<<"[";
            for(unsigned int j = 0 ; j < fronts[k].getCurrentNbChromosomes() ; j++)
                std::cout << " " << fronts[k].getChromosomes()[j].getRank();
            std::cout<< "]"<<std::endl<<std::endl;
        }
        std::cout <<std::endl<<std::endl<<std::endl;
    }
    // Delete last front created because it is empty.
    //fronts.erase(fronts.end());

    return fronts;
}

template<typename T, typename P, typename C>
void NSGAII<T, P, C>::crowdingDistanceAssignement(P* popToAssignCrowdingDistance)
{
    // Number of solutions in popToAssignCrowdingDistance
    unsigned int nbSolutions = popToAssignCrowdingDistance->getCurrentNbChromosomes();

    // Initialize distance
    for (unsigned int i = 0 ; i < nbSolutions ; i++)
        popToAssignCrowdingDistance->getChromosomes()[i].setDistance(0);

    if (popToAssignCrowdingDistance->getChromosomes().empty())
        return;

    Ascending<C> comparator;    // Comaparator using to sort on ascending order each objectives
    unsigned int nbObjective = popToAssignCrowdingDistance->getChromosomes()[0].getNbObjective();
    for (unsigned int m = 0 ; m < nbObjective ; m++)
    {
        // Sort using each objective value
        comparator.index = m;
        std::sort(popToAssignCrowdingDistance->getChromosomes().begin(),
                  popToAssignCrowdingDistance->getChromosomes().end(),
                  comparator);

        // Assigne value min and max of fitness for the objective m
        T minFitnessValue, maxFitnessValue;
        minFitnessValue = popToAssignCrowdingDistance->getChromosomes()[0].getFitness()[m];
        maxFitnessValue = popToAssignCrowdingDistance->getChromosomes()[nbSolutions-1].getFitness()[m];

        // So that boundary point are always selected
        // Extremes chromosomes of the vector are initialized with an infinite distance
        popToAssignCrowdingDistance->getChromosomes()[0].setDistance(std::numeric_limits<double>::max());
        popToAssignCrowdingDistance->getChromosomes()[nbSolutions-1].setDistance(std::numeric_limits<double>::max());

        // For all other points
        for (unsigned int i = 1 ; i < nbSolutions-2 ; i++)
            popToAssignCrowdingDistance->getChromosomes()[i].setDistance(
                                                                         popToAssignCrowdingDistance->getChromosomes()[i].getDistance()
                                                                         + (popToAssignCrowdingDistance->getChromosomes()[i+1].getFitness()[m] - popToAssignCrowdingDistance->getChromosomes()[i-1].getFitness()[m])
                                                                         / (maxFitnessValue - minFitnessValue)
                                                                         );
    }
}

template<typename T, typename P, typename C>
void NSGAII<T, P, C>::addChromosomeWithoutControl(P* pop, const C& chromosome)
{
    if (pop->isFull())
        pop->setNbMaxChromosomes(pop->getNbMaxChromosomes()+1);
    pop->addChromosome(chromosome);
}

template<typename T, typename P, typename C>
void NSGAII<T, P, C>::initialize()
{
    // TODO
    if(this->m_isInitialized)   // already initialized
        return;

    this->releaseMemory();

    // Generate a random population make step that need to be done before running algorithm
    this->m_population = new P;
    this->m_population->generateRandomChromosomes();
    this->m_population->evaluateFitness();

    // Create offspring of the random population
    this->m_offspring = new P;
    this->m_offspring->generateRandomChromosomes(); // TO CHANGE
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
