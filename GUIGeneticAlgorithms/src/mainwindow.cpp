#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(QApplication::applicationName());

    m_paramsDW = new ParamsDockWidget;
    addDockWidget(Qt::LeftDockWidgetArea, m_paramsDW);
    m_solutionsDW = new SolutionListerDockWidget;
    addDockWidget(Qt::BottomDockWidgetArea, m_solutionsDW);

    m_paramsFileName = "";

    // Connect
    connect(ui->action_Search_Params, SIGNAL(triggered()), this, SLOT(openParamsFile()));
    connect(ui->gaPushButton, SIGNAL(released()), this, SLOT(runGAAlgorithm()));
    connect(ui->nsga2PushButton, SIGNAL(released()), this, SLOT(runNSGA2Algorithm()));
    connect(ui->gaPushButton, SIGNAL(pressed()), ui->algorithmProgressBar, SLOT(reset()));
    connect(ui->nsga2PushButton, SIGNAL(pressed()), ui->algorithmProgressBar, SLOT(reset()));

    connect(ui->action_About, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->actionAbout_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(ui->action_Quit, SIGNAL(triggered()), qApp, SLOT(quit()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openParamsFile()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Params File"), "./", tr("Params (*.txt)"));
    if (!filename.isEmpty())
    {
        m_paramsFileName = filename;
        m_paramsDW->setReadParamsFromFileState(true);
    }
}

void MainWindow::runGAAlgorithm()
{
    changePushButtonState(false);

    m_solutionsDW->clearSolutionList();

    try {
        SingleObjectiveGA<int, RouletteWheel<int, int, ChromosomeIntInt>, ChromosomeIntInt> sGa(false);
        if (m_paramsDW->getReadParamsFromFileState())
        {
            if (m_paramsFileName == "") // No file known
            {
                QMessageBox::warning(this,
                                     tr("File Unknown"),
                                     tr("You haven't specified settings file."));

                changePushButtonState();
                return;
            }

            sGa.readParamsFromFile(m_paramsFileName.toStdString());
        }
        else
        {
            sGa.setNbGenerationsWanted(m_paramsDW->getNbGenerationsWanted());
            ChromosomeIntInt::setNbGenes(m_paramsDW->getNbGenes());
            RouletteWheel<int, int, ChromosomeIntInt>::setSNbMaxChromosomes(m_paramsDW->getNbMaxChromosomes());
            RouletteWheel<int, int, ChromosomeIntInt>::setCrossOverProbability(m_paramsDW->getCrossoverProbability());
            RouletteWheel<int, int, ChromosomeIntInt>::setMutateProbability(m_paramsDW->getMutateProbability());
        }

        sGa.initialize();
        performAlgorithm<int, RouletteWheel<int, int, ChromosomeIntInt>, ChromosomeIntInt>(&sGa);
        m_solutionsDW->setSolutionList(formattingSolutions<ChromosomeIntInt>(sGa.getPopulation().getBestSolution()));
    }
    catch (std::runtime_error& e)
    {
        QMessageBox::critical(this,
                              tr("Process Error"),
                              e.what());
    }

    changePushButtonState();
}

void MainWindow::runNSGA2Algorithm()
{
    changePushButtonState(false);

    m_solutionsDW->clearSolutionList();

    try {
        NSGAII<int, TournamentM<int, int, ChromosomeMIntInt>, ChromosomeMIntInt> nsga2(false);
        if (m_paramsDW->getReadParamsFromFileState())
        {
            if (m_paramsFileName == "") // No file known
            {
                QMessageBox::warning(this,
                                     tr("File Unknown"),
                                     tr("You haven't specified settings file."));

                changePushButtonState();
                return;
            }

            nsga2.readParamsFromFile(m_paramsFileName.toStdString());
        }
        else
        {
            nsga2.setNbGenerationsWanted(m_paramsDW->getNbGenerationsWanted());
            ChromosomeMIntInt::setNbGenes(m_paramsDW->getNbGenes());
            TournamentM<int, int, ChromosomeMIntInt>::setSNbMaxChromosomes(m_paramsDW->getNbMaxChromosomes());
            TournamentM<int, int, ChromosomeMIntInt>::setCrossOverProbability(m_paramsDW->getCrossoverProbability());
            TournamentM<int, int, ChromosomeMIntInt>::setMutateProbability(m_paramsDW->getMutateProbability());
        }

        nsga2.initialize();
        performAlgorithm<int, TournamentM<int, int, ChromosomeMIntInt>, ChromosomeMIntInt>(&nsga2);
        m_solutionsDW->setSolutionList(formattingSolutions<ChromosomeMIntInt>(nsga2.getPopulation().getBestSolution()));
    }
    catch (std::runtime_error& e)
    {
        QMessageBox::critical(this,
                              tr("Process Error"),
                              e.what());
    }

    changePushButtonState();
}

void MainWindow::changePushButtonState(bool state)
{
    ui->gaPushButton->setEnabled(state);
    ui->nsga2PushButton->setEnabled(state);
    repaint();
}

template<typename T, typename P, typename C>
void MainWindow::performAlgorithm(GA<T, P, C>* algorithm)
{
    // Run the algorithm if it is initialized
    if (!algorithm->getIfIsInitialized())
        throw std::runtime_error("Algorithm not initialzed !");

    while (algorithm->getIndexCurrentGeneration() <= algorithm->getNbGenerationsWanted())
    {
        ui->algorithmProgressBar->setValue((algorithm->getIndexCurrentGeneration() / (double)algorithm->getNbGenerationsWanted())*100);
        algorithm->runOneGeneration();
    }
}

template<typename C>
std::vector<QString> MainWindow::formattingSolutions(const std::vector<C> solutions)
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

void MainWindow::about()
{
    QMessageBox::about( this,
                        QApplication::applicationName(),
                        "----------- " + QApplication::applicationName() + " -----------"
                        + "<p>" + tr("Version: ") + QApplication::applicationVersion() + "</p>"
                        + "<p><strong>" + tr("Created by :") + "</strong><br>"
                        + "RABERIN Alexandre</p>"
                        + "April 2014 - ..."
                        + "During work placement in Aberystwyth University.");
}
