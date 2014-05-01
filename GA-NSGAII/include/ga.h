#ifndef GA_H
#define GA_H

#include <vector>
#include <fstream>

#include "AlgoString.h"

#include "population.h"

template<typename F, typename P, typename C>
/**
 * @brief The GA class Provide some prototypes of function to run genetic algorithms.
 */
class GA
{
protected:
    bool m_isInitialized;       ///> Indicate if the GA has been initialized.

    int m_nbGenerationsWanted;  ///> Number of generations (iterations) that wil be run in performGA function.
    int m_currentGeneration;    ///> Index of the current generation running.

    P* m_population;            ///> Current population of solutions.

    /**
     * @brief releaseMemory Free all the memory used by the algorithm that haven't been deallocate during the processing.
     */
    virtual void releaseMemory();

public:
    GA();
    virtual ~GA();

    /**
     * @brief initialize Do all things that are needed to do before running the algorithm.
     */
    virtual void initialize() =0;

    /**
     * @brief performGA Run the genetic algorithm.
     * @return the best solution found.
     */
    virtual std::vector< C > performGA() =0;

    /**
     * @brief reset Reset all values in order to do another run of the algorithm.
     */
    virtual void reset() =0;

    /**
     * @brief runOneGeneration Do all steps needed for one generation (iteration), like mutation and crossover operators, etc...
     */
    virtual void runOneGeneration() =0;

    /**
     * @brief readParamsFromFile Read and set params of GA to the values read in the file passed in parameter.
     * @param fileName Name of the file to read.
     */
    virtual void readParamsFromFile(const char* fileName);
    /**
     * @brief readParamsFromFile Read and set params of GA to the values read in the file passed in parameter.
     * @param fileName Name of the file to read.
     */
    virtual void readParamsFromFile(const std::string& fileName);


    ////////////// Accessors/Setters //////////////
    /**
     * @brief setNbGenerationsWanted Set the number of generation that will be run to nbGenerationsWanted.
     * @param nbGenerationsWanted number of generations wanted.
     */
    void setNbGenerationsWanted(const int nbGenerationsWanted) { m_nbGenerationsWanted = nbGenerationsWanted; }

    /**
     * @brief getNbGenerationsWanted Get the number of generation that will be run to nbGenerationsWanted.
     * @return the number of generations wanted for the run.
     */
    int getNbGenerationsWanted() const { return m_nbGenerationsWanted; }

    /**
     * @brief getIndexCurrentGeneration Get the index of the current generation.
     * @return the index of the current generation.
     */
    int getIndexCurrentGeneration() const { return m_currentGeneration; }

    /**
     * @brief getIfIsInitialized Get the state of the algorithm : initialized or not.
     * @return Algorithm initialization state.
     */
    bool getIfIsInitialized() const { return m_isInitialized; }

    /**
     * @brief getPopulation Get the current population of the algorithm.
     * @return m_population content.
     */
    virtual const P& getPopulation() const {return *m_population;}
};

template<typename F, typename P, typename C>
GA<F, P, C>::GA()
    : m_isInitialized(false)
    , m_nbGenerationsWanted(100)
    , m_population(nullptr)
{
}

template<typename F, typename P, typename C>
GA<F, P, C>::~GA()
{
    releaseMemory();
}

template<typename F, typename P, typename C>
void GA<F, P, C>::releaseMemory()
{
    if (m_population)
        delete m_population;
    m_population = nullptr;
}

template<typename F, typename P, typename C>
void GA<F, P, C>::readParamsFromFile(const char* fileName)
{
    if (m_isInitialized)
        throw std::runtime_error("Impossible to set parameters. Please reset GA before.");

    std::ifstream file(fileName, std::ios::in);  // Open file

    if(file)  // Open Success
    {
        while (!file.eof())
        {
            std::string line;           // One line of the file
            std::getline(file, line);   // Recuperate one line

            std::vector<std::string> lineSplited = split(line, '=');    // Split the line on char '='
            for (int i = 0 ; i < lineSplited.size() ; i++)              // Delete all space at the beginning and end of each splited strings
                lineSplited[i] = trim(lineSplited[i]);

            if (lineSplited.size() < 2) // If there aren't 2 arguments (label + value) => error
                throw std::runtime_error("There is an error in file.");

            // Recover the label without space and in lower case
            std::string lineLabel = "";
            std::vector<std::string> lineLabelSplited = split(lineSplited[0], ' ');
            for (int i = 0 ; i < lineLabelSplited.size() ; i++)
            {
                lineLabelSplited[i] = trim(lineLabelSplited[i]);
                lineLabel += lineLabelSplited[i];
            }
            lineLabel = lowerCase(lineLabel);

            // Check the property to update
            if (lineLabel == "probabilitycrossover")
                P::setCrossOverProbability(getNumber<double>(lineSplited[1]));
            else if (lineLabel == "probabilitymutate")
                P::setMutateProbability(getNumber<double>(lineSplited[1]));
            else if (lineLabel == "numbergenerationswanted")
                m_nbGenerationsWanted = getNumber<int>(lineSplited[1]);
            else if (lineLabel == "numbermaxchromosomes")
                P::setSNbMaxChromosomes(getNumber<int>(lineSplited[1]));
            else if (lineLabel == "numbergenes")
                C::setNbGenes(getNumber<int>(lineSplited[1]));
            else if (lineLabel == "proportionalkeeping")
                P::setProportionalKeeping(getNumber<double>(lineSplited[1]));
            else
                throw std::runtime_error("There is an error in file.");
        }

        file.close();  // Close file
    }
    else
        throw std::runtime_error("Impossible to open file or file doesn't exist!");
}

template<typename F, typename P, typename C>
void GA<F, P, C>::readParamsFromFile(const std::string& fileName)
{
    readParamsFromFile(fileName.c_str());
}

#endif // GA_H
