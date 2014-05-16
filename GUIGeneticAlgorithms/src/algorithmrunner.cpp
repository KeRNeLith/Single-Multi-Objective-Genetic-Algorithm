#include "algorithmrunner.h"

AlgorithmRunner::AlgorithmRunner(MainWindow *mainWindow)
    : m_breakAlgorithm(false)
    , m_mainwindow(mainWindow)
{
    connect(this, SIGNAL(fileUnknown()), mainWindow, SLOT(showFileUnknownMessage()));
    connect(this, SIGNAL(algorithmFailure(const QString&)), mainWindow, SLOT(showAlgorithmFailureMessage(const QString&)));
    connect(this, SIGNAL(updateProgressBar(int)), mainWindow, SLOT(updateProgressBarValue(int)));
}

void AlgorithmRunner::runAlgorithm(const QString& parameter)
{
    // Parameter handling
    std::vector<std::string> splitedParameter = split(parameter.toStdString(), ' ');
    std::string identificator = "";
    for (unsigned int i = 0 ; i < splitedParameter.size() ; i++)
    {
        splitedParameter[i] = trim(splitedParameter[i]);
        identificator += splitedParameter[i];
    }

    if (identificator == "ga")
        configureAndRunAlgorithm<int, RouletteWheel<int, int, ChromosomeIntInt>, ChromosomeIntInt>(createSingleObjectiveAlgorithm<int, RouletteWheel<int, int, ChromosomeIntInt>, ChromosomeIntInt>());
    else if (identificator == "nsga2")
        configureAndRunAlgorithm<int, TournamentM<int, int, ChromosomeMDoubleInt>, ChromosomeMDoubleInt>(createNSGAIIAlgorithm<int, TournamentM<int, int, ChromosomeMDoubleInt>, ChromosomeMDoubleInt>());
    else
        emit algorithmBroken();
}

template<typename T, typename P, typename C>
void AlgorithmRunner::configureAndRunAlgorithm(smoga::GA<T, P, C> *algorithm)
{
    try {
        if (m_mainwindow->getParamsDockWidget()->getReadParamsFromFileState())
        {
            if (m_mainwindow->getParamsFileName() == "") // No file known
            {
                emit fileUnknown();
                emit algorithmBroken();
                delete algorithm;
                return;
            }

            algorithm->readParamsFromFile(m_mainwindow->getParamsFileName().toStdString());
        }
        else
        {
            algorithm->setNbGenerationsWanted(m_mainwindow->getParamsDockWidget()->getNbGenerationsWanted());
            C::setNbGenes(m_mainwindow->getParamsDockWidget()->getNbGenes());
            P::setSNbMaxChromosomes(m_mainwindow->getParamsDockWidget()->getNbMaxChromosomes());
            P::setCrossOverProbability(m_mainwindow->getParamsDockWidget()->getCrossoverProbability());
            P::setMutateProbability(m_mainwindow->getParamsDockWidget()->getMutateProbability());
        }

        algorithm->initialize();
        if (performAlgorithm<T, P, C>(algorithm))
        {
            if (dynamic_cast< smoga::NSGAII<T, P, C>* > (algorithm))
                emit algorithmExecuted(formattingNSGAIISolutions<C>(algorithm->getPopulation().getBestSolution()));
            else
                emit algorithmExecuted(formattingSingleObjectiveSolution<C>(algorithm->getPopulation().getBestSolution()));
        }
        else
            delete algorithm;
    }
    catch (std::runtime_error& e)
    {
        emit algorithmFailure(e.what());
        emit algorithmBroken();
        delete algorithm;
        return;
    }
}

template<typename T, typename P, typename C>
bool AlgorithmRunner::performAlgorithm(smoga::GA<T, P, C>* algorithm)
{
    // Run the algorithm if it is initialized
    if (!algorithm->getIfIsInitialized())
        throw std::runtime_error("Algorithm not initialzed !");

    while (algorithm->getIndexCurrentGeneration() <= algorithm->getNbGenerationsWanted())
    {
        if (m_breakAlgorithm)
        {
            emit algorithmBroken();
            return false;
        }

        emit updateProgressBar(algorithm->getIndexCurrentGeneration() / (double)algorithm->getNbGenerationsWanted()*100);
        algorithm->runOneGeneration();
        QString fileName = "generation" + QString::number(algorithm->getIndexCurrentGeneration()-1) + ".txt";

        try {
            algorithm->dumpToFile(fileName.toStdString());
        }
        catch(std::runtime_error& e)
        {
            emit algorithmFailure(e.what());
            emit algorithmBroken();
            return false;
        }

        if (dynamic_cast< smoga::NSGAII<T, P, C>* > (algorithm) && !m_breakAlgorithm)
            emit needToUpdateGraph(fileName);
    }

    return true;
}

template<typename T, typename P, typename C>
smoga::NSGAII<T, P, C>* AlgorithmRunner::createNSGAIIAlgorithm()
{
    return new smoga::NSGAII<T, P, C>(false);
}

template<typename T, typename P, typename C>
smoga::SingleObjectiveGA<T, P, C>* AlgorithmRunner::createSingleObjectiveAlgorithm()
{
    return new smoga::SingleObjectiveGA<T, P, C>(false);
}

template<typename C>
std::vector<QString> AlgorithmRunner::formattingSingleObjectiveSolution(const std::vector<C> solutions)
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

        datas += " => | ";
        datas += QString::number(it->getFitness()[0]);
        datas += " |";

        stringSolutions.push_back(datas);

        datas = "";
        genes.clear();
        index++;
    }

    return stringSolutions;
}

template<typename C>
std::vector<QString> AlgorithmRunner::formattingNSGAIISolutions(const std::vector<C> solutions)
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

        double value = getDoubleFromBinary(genes);

        datas += " => ";
        datas += QString::number(value);

        datas += " => \t| ";
        for (unsigned int j = 0 ; j < it->getFitness().size() ; j++)
        {
            datas += QString::number(it->getFitness()[j]);
            datas += " | ";
        }

        stringSolutions.push_back(datas);

        datas = "";
        genes.clear();
        index++;
    }

    return stringSolutions;
}

void AlgorithmRunner::breakAlgorithm(bool breakLoop)
{
    m_breakAlgorithm = breakLoop;
}
