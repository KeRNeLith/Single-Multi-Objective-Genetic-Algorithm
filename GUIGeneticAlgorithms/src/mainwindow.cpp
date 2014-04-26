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

    statusBar()->showMessage(tr("Ready..."), 3000);
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

    statusBar()->showMessage(tr("File ready to be load in the next algorithm run ..."), 7000);
}

void MainWindow::runGAAlgorithm()
{
    initThreadAlgorithm();

    emit launchAlgorithm(QString("ga"));

    statusBar()->showMessage(tr("Launch GA algorithm..."), 2000);
}

void MainWindow::runNSGA2Algorithm()
{
    initThreadAlgorithm();

    emit launchAlgorithm(QString("nsga2"));

    statusBar()->showMessage(tr("Launch NSGA-II algorithm..."), 2000);
}

void MainWindow::handleResults(const std::vector<QString> &result)
{
    m_mainWindowThread.quit();
    m_solutionsDW->setSolutionList(result);

    changePushButtonState();
    statusBar()->showMessage(tr("Algorithm finished..."), 4000);
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

void MainWindow::initThreadAlgorithm()
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
}

void MainWindow::about()
{
    QMessageBox::about( this,
                        QApplication::applicationName(),
                        "----------- " + QApplication::applicationName() + " -----------"
                        + "<p>" + tr("Version: ") + QApplication::applicationVersion() + "</p>"
                        + "<p><strong>" + tr("Created by :") + "</strong><br>"
                        + "RABERIN Alexandre</p>"
                        + "April 2014 - ...<br>"
                        + "During work placement in Aberystwyth University.");
}
