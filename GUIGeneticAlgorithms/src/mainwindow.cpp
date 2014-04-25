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

    m_paramsFileName = "";

    // Connect
    connect(ui->action_Search_Params, SIGNAL(triggered()), this, SLOT(openParamsFile()));
    connect(ui->gaPushButton, SIGNAL(clicked()), this, SLOT(runGAAlgorithm()));
    connect(ui->nsga2PushButton, SIGNAL(clicked()), this, SLOT(runNSGA2Algorithm()));

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
        m_paramsFileName = filename;
}

void MainWindow::runGAAlgorithm()
{
    changePushButtonState(false);

    try {
        SingleObjectiveGA<int, RouletteWheel<int, int, ChromosomeIntInt>, ChromosomeIntInt> sGa;
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
        int ret = sGa.performGA()[0].getFitness()[0];
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

    try {
        NSGAII<int, TournamentM<int, int, ChromosomeMIntInt>, ChromosomeMIntInt> nsga2;
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
        nsga2.performGA();
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
