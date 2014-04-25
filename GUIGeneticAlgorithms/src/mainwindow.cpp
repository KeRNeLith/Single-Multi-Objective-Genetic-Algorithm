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
    changePushButtonState();

    //SingleObjectiveGA<int, RouletteWheel<int, int, ChromosomeIntInt>, ChromosomeIntInt> sGa;
    if (m_paramsDW->getReadParamsFromFileState())
    {
        if (m_paramsFileName == "") // no file known
        {
            QMessageBox::warning(this,
                                 tr("File Unknown"),
                                 tr("You haven't specified settings file."));

            changePushButtonState();
            return;
        }

        //sGa.readParamsFromFile(m_paramsFileName.toStdString());
    }
    else
    {
        /*sGa.setNbGenerationsWanted(m_paramsDW->getNbGenerationsWanted());
        ChromosomeIntInt::setNbGenes(m_paramsDW->getNbGenes());
        RouletteWheel<ChromosomeIntInt>::setSNbMaxChromosomes(m_paramsDW->getNbMaxChromosomes());
        RouletteWheel<ChromosomeIntInt>::setCrossOverProbability(m_paramsDW->getCrossoverProbability());
        RouletteWheel<ChromosomeIntInt>::setMutateProbability(m_paramsDW->getMutateProbability());*/
    }

    //sGa.initialize();
    //int ret = sGa.performGA()[0].getFitness()[0];
    changePushButtonState();
}

void MainWindow::runNSGA2Algorithm()
{
    changePushButtonState();

    // TODO

    changePushButtonState();
}

void MainWindow::changePushButtonState()
{
    ui->gaPushButton->setEnabled(!ui->gaPushButton->isEnabled());
    ui->nsga2PushButton->setEnabled(!ui->nsga2PushButton->isEnabled());
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
