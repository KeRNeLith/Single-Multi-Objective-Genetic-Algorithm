#include <iostream>

#include "singleobjectivega.h"
#include "nsgaii.h"

#include "roulettewheel.h"
#include "tournamentm.h"
#include "chromosomeintint.h"
#include "chromosomemintint.h"

int main()
{
    try {
        ////////////////// SINGLE OBJECTIVE GA (Roulette Wheel) //////////////////
        // Count the number of one in a serie of bits
        // Return the solution that for a given number of bits have the higher number of bits to 1.

        // Meaning of templates parameters :
        // first : It's the type which will be use for fitnesses of chromosomes.
        // second : It's the type of population used (mainly for the manner of selection for the breeding), the 3 templates parameters are the following :
        //          \-> first : It's still the type used for fitnesses.
        //          \-> second : It's the type of data used the chromosomes.
        //          \-> third : It's the type of chromosomes used.
        // third : It's the type of chromosomes used.
        /*SingleObjectiveGA<int, RouletteWheel<int, int, ChromosomeIntInt>, ChromosomeIntInt> sGa;
        sGa.readParamsFromFile("params.txt");

        // Example of changing parameters to have a specific configuration of the GA used.
        //sGa.setElistism(false);
        //sGa.setNbGenerationsWanted(500);
        //ChromosomeIntInt::setNbGenes(100);
        //RouletteWheel<int, int, ChromosomeIntInt>::setSNbMaxChromosomes(50);
        //RouletteWheel<int, int, ChromosomeIntInt>::setProportionalKeeping(0.3);
        //RouletteWheel<int, int, ChromosomeIntInt>::setCrossOverProbability(0.35);
        //RouletteWheel<int, int, ChromosomeIntInt>::setMutateProbability(0.03);

        sGa.initialize();
        int ret = sGa.performGA()[0].getFitness()[0];
        std::cout << std::endl << "Result : " << ret << std::endl;*/

        ////////////////// MULTI OBJECTIVE GA (NSGA-II) //////////////////
        NSGAII<int, TournamentM<int, int, ChromosomeMIntInt>, ChromosomeMIntInt> nsga2;
        nsga2.readParamsFromFile("params.txt");

        // Example of changing parameters to have a specific configuration of the GA used.
        // Same method, described above, except there isn't function setElitism and setProportionalKeeping.

        nsga2.initialize();
        std::vector<ChromosomeMIntInt> results = nsga2.performGA();
        std::cout << std::endl << std::endl << "Solutions : " << std::endl;
        for (unsigned int i = 0 ; i < results.size() ; i++)
            std::cout << "(" << i+1 << ")\t Rank : " << results[i].getRank() << " || Distance : " << results[i].getDistance() << std::endl;

        std::cout << std::endl << "Done... !" << std::endl;
    }
    catch(std::runtime_error& e)
    {
        std::cout << e.what() << std::endl;
    }

    return EXIT_SUCCESS;
}

