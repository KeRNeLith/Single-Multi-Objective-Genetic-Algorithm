#include <iostream>
#include "singleobjectivega.h"
#include "roulettewheel.h"
#include "chromosomeintint.h"

using namespace std;

int main()
{
    try {
        SingleObjectiveGA<int, RouletteWheel<ChromosomeIntInt>, ChromosomeIntInt> sGa;

        //sGa.setElistism(true);
        //sGa.setNbGenerationsWanted(500);
        //ChromosomeIntInt::setNbGenes(100);
        //RouletteWheel<ChromosomeIntInt>::setNbMaxChromosomes(50);
        //RouletteWheel<ChromosomeIntInt>::setProportionalKeeping(0.3);

        sGa.initialize();
        int ret = sGa.performGA()[0].getFitness();
        cout << "Result : " << ret << endl;
    }
    catch(std::runtime_error& e)
    {
        cout << e.what() << endl;
    }

    return 0;
}

