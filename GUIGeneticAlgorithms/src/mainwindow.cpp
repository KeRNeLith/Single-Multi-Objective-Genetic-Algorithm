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

    // Register Meta Type for signals and solts
    qRegisterMetaType<StringVector>("StringVector");
}

MainWindow::~MainWindow()
{
    delete ui;
    m_mainWindowThread.quit();
    m_mainWindowThread.wait();
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

    AlgorithmRunner* algorithmRunner = new AlgorithmRunner(this);
    algorithmRunner->moveToThread(&m_mainWindowThread);
    // To delete the pointer later
    connect(&m_mainWindowThread, SIGNAL(finished()), algorithmRunner, SLOT(deleteLater()));
    // To launch algorithm
    connect(this, SIGNAL(launchAlgorithm(QString)), algorithmRunner, SLOT(runAlgorithm(QString)));
    // To recover results
    connect(algorithmRunner, SIGNAL(algorithmExecuted(std::vector<QString>)), this, SLOT(handleResults(std::vector<QString>)));
    m_mainWindowThread.start();

    emit launchAlgorithm(QString());
}

void MainWindow::runNSGA2Algorithm()
{
    /*changePushButtonState(false);

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

    changePushButtonState();*/
}

void MainWindow::handleResults(const std::vector<QString> &result)
{
    m_mainWindowThread.quit();
    m_solutionsDW->setSolutionList(result);

    changePushButtonState();
}

void MainWindow::showFileUnknownMessage()
{
    QMessageBox::warning(this,
                         tr("File Unknown"),
                         tr("You haven't specified settings file."));
}

void MainWindow::showAlgorithmFailureMessage(const char *message)
{
    QMessageBox::critical(this,
                          tr("Process Error"),
                          message);
}

void MainWindow::updateProgressBarValue(int value)
{
    ui->algorithmProgressBar->setValue(value);
}

void MainWindow::changePushButtonState(bool state)
{
    ui->gaPushButton->setEnabled(state);
    ui->nsga2PushButton->setEnabled(state);
    repaint();
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
