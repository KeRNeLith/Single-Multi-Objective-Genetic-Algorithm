#include <iostream>
#include "ga.h"
#include "roulettewheel.h"
#include "chromosomeintint.h"

using namespace std;

int main()
{
    try {
        GA<int, RouletteWheel<ChromosomeIntInt>, ChromosomeIntInt> ga;

        //ga.setNbGenerationsWanted(500);
        //ChromosomeIntInt::setNbGenes(100);
        //RouletteWheel<ChromosomeIntInt>::setNbMaxChromosomes(50);
        //RouletteWheel<ChromosomeIntInt>::setProportionalKeeping(0.3);

        ga.initialize();
        int ret = ga.performGA();
        cout << "Result : " << ret << endl;
    }
    catch(std::runtime_error& e)
    {
        cout << e.what() << endl;
    }

    return 0;
}

