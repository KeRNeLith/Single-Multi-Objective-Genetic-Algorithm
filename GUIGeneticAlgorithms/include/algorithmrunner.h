#ifndef ALGORITHMRUNNER_H
#define ALGORITHMRUNNER_H

#include <vector>
#include <string>

#include <QObject>

#include "General.h"
#include "AlgoString.h"

#include "ga.h"
#include "singleobjectivega.h"
#include "nsgaii.h"

#include "roulettewheel.h"
#include "chromosomeintint.h"

#include "tournamentm.h"
#include "chromosomemdoubleint.h"

#include "mainwindow.h"

class MainWindow;

class AlgorithmRunner
        : public QObject
{
    Q_OBJECT

private:
    MainWindow* m_mainwindow;

    template<typename T, typename P, typename C>
    void performAlgorithm(GA<T, P, C>* algorithm);

    template<typename T, typename P, typename C>
    void configureAndRunAlgorithm(GA<T, P, C>* algorithm);

    template<typename T, typename P, typename C>
    NSGAII<T, P, C>* createNSGAIIAlgorithm();

    template<typename T, typename P, typename C>
    SingleObjectiveGA<T, P, C>* createSingleObjectiveAlgorithm();

    template<typename C>
    std::vector<QString> formattingSingleObjectiveSolution(const std::vector<C> solutions);
    template<typename C>
    std::vector<QString> formattingNSGAIISolutions(const std::vector<C> solutions);

public slots:
    void runAlgorithm(const QString& parameter);

signals:
    void algorithmExecuted(const std::vector<QString>& result);
    void fileUnknown();
    void algorithmFailure(const char* message);
    void updateProgressBar(int progress);
    void needToUpdateGraph();

public:
    AlgorithmRunner(MainWindow *mainWindow);
};

#endif // ALGORITHMRUNNER_H
