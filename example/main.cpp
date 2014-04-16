#include <iostream>
#include "singleobjectivega.h"
#include "roulettewheel.h"
#include "chromosomeintint.h"

using namespace std;

int main()
{
    try {

        // Count the number of one in a serie of bits
        // Return the solution that for a given number of bits have the higher number of bits to 1.

        // Meaning of templates parameters :
        // first : It's the type which will be use for fitnesses of chromosomes.
        // second : It's the type of population used (mainly for the manner of selection for the breeding), the 3 templates parameters are the following :
        //          \-> first : It's still the type used for fitnesses.
        //          \-> second : It's the type of data used the chromosomes.
        //          \-> third : It's the type of chromosomes used.
        // third : It's the type of chromosomes used.
        SingleObjectiveGA<int, RouletteWheel<int, int, ChromosomeIntInt>, ChromosomeIntInt> sGa;

        // Example of changing parameters to have a specific configuration of the GA used.
        //sGa.setElistism(false);
        //sGa.setNbGenerationsWanted(500);
        //ChromosomeIntInt::setNbGenes(100);
        //RouletteWheel<ChromosomeIntInt>::setNbMaxChromosomes(50);
        //RouletteWheel<ChromosomeIntInt>::setProportionalKeeping(0.3);
        //RouletteWheel<ChromosomeIntInt>::setCrossOverProbability(0.35);
        //RouletteWheel<ChromosomeIntInt>::setMutateProbability(0.03);

        sGa.initialize();
        int ret = sGa.performGA()[0].getFitness()[0];
        cout << "Result : " << ret << endl;
    }
    catch(std::runtime_error& e)
    {
        cout << e.what() << endl;
    }

    return 0;
}

