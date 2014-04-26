#include "algorithmrunner.h"

AlgorithmRunner::AlgorithmRunner(MainWindow *mainWindow)
    : m_mainwindow(mainWindow)
{
    connect(this, SIGNAL(fileUnknown()), mainWindow, SLOT(showFileUnknownMessage()));
    connect(this, SIGNAL(algorithmFailure(const char*)), mainWindow, SLOT(showAlgorithmFailureMessage(const char*)));
    connect(this, SIGNAL(updateProgressBar(int)), mainWindow, SLOT(updateProgressBarValue(int)));
}

void AlgorithmRunner::runAlgorithm(const QString& parameter)
{
    SingleObjectiveGA<int, RouletteWheel<int, int, ChromosomeIntInt>, ChromosomeIntInt> sGa(false);
    try {
        if (m_mainwindow->getParamsDockWidget()->getReadParamsFromFileState())
        {
            if (m_mainwindow->getParamsFileName() == "") // No file known
            {
                emit fileUnknown();
                emit algorithmExecuted(std::vector<QString>());
                return;
            }

            sGa.readParamsFromFile(m_mainwindow->getParamsFileName().toStdString());
        }
        else
        {
            sGa.setNbGenerationsWanted(m_mainwindow->getParamsDockWidget()->getNbGenerationsWanted());
            ChromosomeIntInt::setNbGenes(m_mainwindow->getParamsDockWidget()->getNbGenes());
            RouletteWheel<int, int, ChromosomeIntInt>::setSNbMaxChromosomes(m_mainwindow->getParamsDockWidget()->getNbMaxChromosomes());
            RouletteWheel<int, int, ChromosomeIntInt>::setCrossOverProbability(m_mainwindow->getParamsDockWidget()->getCrossoverProbability());
            RouletteWheel<int, int, ChromosomeIntInt>::setMutateProbability(m_mainwindow->getParamsDockWidget()->getMutateProbability());
        }

        sGa.initialize();
        performAlgorithm<int, RouletteWheel<int, int, ChromosomeIntInt>, ChromosomeIntInt>(&sGa);
    }
    catch (std::runtime_error& e)
    {
        emit algorithmFailure(e.what());
        emit algorithmExecuted(std::vector<QString>());
        return;
    }

    emit algorithmExecuted(formattingSolutions<ChromosomeIntInt>(sGa.getPopulation().getBestSolution()));
}

template<typename T, typename P, typename C>
void AlgorithmRunner::performAlgorithm(GA<T, P, C>* algorithm)
{
    // Run the algorithm if it is initialized
    if (!algorithm->getIfIsInitialized())
        throw std::runtime_error("Algorithm not initialzed !");

    while (algorithm->getIndexCurrentGeneration() <= algorithm->getNbGenerationsWanted())
    {
        emit updateProgressBar(algorithm->getIndexCurrentGeneration() / (double)algorithm->getNbGenerationsWanted()*100);
        algorithm->runOneGeneration();
    }
}

template<typename C>
std::vector<QString> AlgorithmRunner::formattingSolutions(const std::vector<C> solutions)
{
    std::vector<QString> stringSolutions;

    QString datas = "";
    std::vector<int> genes;
    int index = 0;
    for (auto it = solutions.begin() ; it != solutions.end() ; it++)
    {
        genes = it->getDatas();

        datas += QString::number(index) += " => ";

        for (auto it2 = genes.begin() ; it2 != genes.end() ; it2++)
            datas += QString::number(*it2);
        stringSolutions.push_back(datas);

        datas = "";
        genes.clear();
        index++;
    }

    return stringSolutions;
}
