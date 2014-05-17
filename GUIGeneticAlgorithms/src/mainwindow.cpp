#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_isPaused(false)
    , m_chrono(nullptr)
    , m_chronoTimeElapsed(nullptr)
    , m_chronoTimer(nullptr)
{
    ui->setupUi(this);
    setWindowTitle(QApplication::applicationName());

    m_paramsDW = new ParamsDockWidget;
    addDockWidget(Qt::LeftDockWidgetArea, m_paramsDW);
    m_paramsDockVisibility = m_paramsDW->toggleViewAction();
    m_paramsDockVisibility->setCheckable(true);
    m_paramsDockVisibility->setChecked(true);
    ui->menuDock_Widget->addAction(m_paramsDockVisibility);
    m_solutionsDW = new SolutionListerDockWidget;

    addDockWidget(Qt::BottomDockWidgetArea, m_solutionsDW);
    m_solutionDockVisibility = m_solutionsDW->toggleViewAction();
    m_solutionDockVisibility->setCheckable(true);
    m_solutionDockVisibility->setChecked(true);
    ui->menuDock_Widget->addAction(m_solutionDockVisibility);

    m_paretoOptimalFrontW = new ParetoOptimalFrontWidget;

    m_paramsFileName = "";
    m_graphFileName = "";

    // Connect
    connect(ui->action_Search_Params, SIGNAL(triggered()), this, SLOT(openParamsFile()));
    connect(ui->action_Load_Graph, SIGNAL(triggered()), this, SLOT(openGraphFile()));
    connect(ui->gaPushButton, SIGNAL(released()), this, SLOT(runGAAlgorithm()));
    connect(ui->nsga2PushButton, SIGNAL(released()), this, SLOT(runNSGA2Algorithm()));
    connect(ui->gaPushButton, SIGNAL(pressed()), ui->algorithmProgressBar, SLOT(reset()));
    connect(ui->nsga2PushButton, SIGNAL(pressed()), ui->algorithmProgressBar, SLOT(reset()));
    connect(ui->stopPushButton, SIGNAL(pressed()), this, SLOT(breakAlgorithm()));
    connect(ui->pausePushButton, SIGNAL(pressed()), this, SLOT(pauseResumeAlgorithm()));

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
    delete m_chrono;
    delete m_chronoTimeElapsed;
    delete m_chronoTimer;
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

void MainWindow::openGraphFile()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Graph File"), "./", tr("Graph Descriptors (*.txt)"));
    if (!filename.isEmpty())
    {
        m_graphFileName = filename;
        loadGraph();
    }
}

void MainWindow::runGAAlgorithm()
{
    initThreadAlgorithm();

    emit launchAlgorithm(QString("ga"));

    resetChrono();
    startChrono();

    statusBar()->showMessage(tr("Launch GA algorithm..."), 2000);
}

void MainWindow::runNSGA2Algorithm()
{
    initThreadAlgorithm();

    emit launchAlgorithm(QString("nsga2"));

    resetChrono();
    startChrono();

    statusBar()->showMessage(tr("Launch NSGA-II algorithm..."), 2000);
}

void MainWindow::breakAlgorithm()
{
    ui->stopPushButton->setEnabled(false);
    ui->pausePushButton->setEnabled(false);
    repaint();
    emit breakCurrentAlgorithm(true);
    statusBar()->showMessage(tr("Algorithm will be break..."), 1000);
}

void MainWindow::pauseResumeAlgorithm()
{
    m_isPaused = !m_isPaused;
    if (!m_isPaused)
    {
        startChrono();
        ui->pausePushButton->setText("Pause");
        statusBar()->showMessage(tr("Resume Algorithm..."), 2000);
    }
    else
    {
        stopChrono();
        ui->pausePushButton->setText("Resume");
        statusBar()->showMessage(tr("Algorithm will be pause..."), 2000);
    }
    repaint();
    emit algorithmPauseState(m_isPaused);
}

void MainWindow::handleResults(const std::vector<QString> &result)
{
    stopChrono();

    m_mainWindowThread.quit();
    m_solutionsDW->setSolutionList(result);

    changePushButtonState();
    statusBar()->showMessage(tr("Algorithm finished..."), 4000);
}

void MainWindow::handleAlgorithmProblem()
{
    stopChrono();
    m_mainWindowThread.quit();

    changePushButtonState();
    statusBar()->showMessage(tr("Algorithm failure..."), 4000);
}

void MainWindow::showFileUnknownMessage()
{
    QMessageBox::warning(this,
                         tr("File Unknown"),
                         tr("You haven't specified settings file."));
}

void MainWindow::showAlgorithmFailureMessage(const QString &message)
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
    ui->stopPushButton->setEnabled(!state);
    ui->pausePushButton->setEnabled(!state);
    repaint();
}

void MainWindow::initThreadAlgorithm()
{
    changePushButtonState(false);
    ui->pausePushButton->setText("Pause");
    m_isPaused = false;
    repaint();

    m_solutionsDW->clearSolutionList();

    AlgorithmRunner* algorithmRunner = new AlgorithmRunner(this);
    algorithmRunner->moveToThread(&m_mainWindowThread);
    // To delete the pointer later
    connect(&m_mainWindowThread, SIGNAL(finished()), algorithmRunner, SLOT(deleteLater()));
    // To launch algorithm
    connect(this, SIGNAL(launchAlgorithm(QString)), algorithmRunner, SLOT(runAlgorithm(QString)));
    // To stop algorithm
    connect(this, SIGNAL(breakCurrentAlgorithm(bool)), algorithmRunner, SLOT(breakAlgorithm(bool)), Qt::DirectConnection);
    // To pause/resume algorithm
    connect(this, SIGNAL(algorithmPauseState(bool)), algorithmRunner, SLOT(setPause(bool)), Qt::DirectConnection);

    // To recover results
    connect(algorithmRunner, SIGNAL(algorithmExecuted(std::vector<QString>)), this, SLOT(handleResults(std::vector<QString>)));
    // When algorithm encounter problems
    connect(algorithmRunner, SIGNAL(algorithmBroken()), this, SLOT(handleAlgorithmProblem()));

    // To update graph drawing
    connect(algorithmRunner, SIGNAL(needToUpdateGraph(const QString&)), this, SLOT(updateParetoOptimalFrontWidget(const QString&)));
    m_mainWindowThread.start();
}

void MainWindow::updateParetoOptimalFrontWidget(const QString& fileName)
{
    m_paretoOptimalFrontW->show();

    try {
        m_paretoOptimalFrontW->loadFile(fileName.toStdString());
    }
    catch(std::runtime_error& e)
    {
        QMessageBox::critical(this,
                              tr("Incorrect File"),
                              e.what());
    }
}

void MainWindow::loadGraph()
{
    if (m_graphFileName == "")
    {
        QMessageBox::warning(this,
                             tr("Unknown File"),
                             tr("No file specified!"));
        return;
    }

    m_paretoOptimalFrontW->show();

    try {
        m_paretoOptimalFrontW->loadFile(m_graphFileName.toStdString());
    }
    catch(std::runtime_error& e)
    {
        QMessageBox::critical(this,
                              tr("Incorrect File"),
                              e.what());
    }

    statusBar()->showMessage(tr("Graph Loaded..."), 3000);
}

void MainWindow::updateGUIChrono()
{
    QTime newChronoTime(0, 0, 0);
    newChronoTime = m_chronoTimeElapsed->addMSecs(m_chrono->elapsed());

    ui->timeElapsed->setText(newChronoTime.toString("h:mm:ss"));
}

void MainWindow::resetChrono()
{
    // m_chrono count time, m_chronoTimeElapsed count time elapsed and m_chronoTimer for display every second
    if (m_chrono)
        delete m_chrono;
    m_chrono = new QTime(0, 0, 0);
    if (m_chronoTimeElapsed)
        delete m_chronoTimeElapsed;
    m_chronoTimeElapsed = new QTime(0, 0, 0);
    if (!m_chronoTimer)
        delete m_chronoTimer;
    m_chronoTimer = new QTimer();

    connect(m_chronoTimer, SIGNAL(timeout()), this, SLOT(updateGUIChrono()));
}

void MainWindow::startChrono()
{
    m_chrono->start();             // Launch chrono
    m_chronoTimer->start(1000);    // Timer 1000 miliseconds
    updateGUIChrono();
}

void MainWindow::stopChrono()
{
    m_chronoTimer->stop();   // Stop timer used for GUI update
    *m_chronoTimeElapsed = m_chronoTimeElapsed->addMSecs(m_chrono->elapsed());
    delete m_chrono;
    m_chrono = new QTime;
    updateGUIChrono();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    if (m_mainWindowThread.isRunning())
    {
        int result = QMessageBox::warning(this,
                                          tr("Close Warning"),
                                          tr("Would you close the application while an algorithm is running?<br><br>The algorithm will finish his current loop BEFORE!"),
                                          QMessageBox::Yes,
                                          QMessageBox::No);

        switch (result)
        {
        case QMessageBox::Yes:
            breakAlgorithm();
            m_mainWindowThread.quit();
            m_mainWindowThread.wait();
            break;
        default:
            event->ignore();
            return;
        }
    }

    m_paretoOptimalFrontW->close();

    QMainWindow::closeEvent(event);
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
