#ifndef ALGORITHMRUNNER_H
#define ALGORITHMRUNNER_H

#include <vector>
#include <string>

#include <QObject>
#include <QWaitCondition>
#include <QMutex>

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
    bool m_breakAlgorithm;
    bool m_isInPause;

    MainWindow* m_mainwindow;

    QWaitCondition m_pauseCondition;
    QMutex m_mutex;

    template<typename T, typename P, typename C>
    bool performAlgorithm(smoga::GA<T, P, C>* algorithm);

    template<typename T, typename P, typename C>
    void configureAndRunAlgorithm(smoga::GA<T, P, C>* algorithm);

    template<typename T, typename P, typename C>
    smoga::NSGAII<T, P, C>* createNSGAIIAlgorithm();

    template<typename T, typename P, typename C>
    smoga::SingleObjectiveGA<T, P, C>* createSingleObjectiveAlgorithm();

    template<typename C>
    std::vector<QString> formattingSingleObjectiveSolution(const std::vector<C> solutions);
    template<typename C>
    std::vector<QString> formattingNSGAIISolutions(const std::vector<C> solutions);

public slots:
    void runAlgorithm(const QString& parameter);
    void breakAlgorithm(bool breakLoop);
    void setPause(bool state);

signals:
    void algorithmBroken();
    void algorithmExecuted(const std::vector<QString>& result);
    void fileUnknown();
    void algorithmFailure(const QString& message);
    void updateProgressBar(int progress);
    void needToUpdateGraph(const QString& fileName);

public:
    AlgorithmRunner(MainWindow *mainWindow);
};

#endif // ALGORITHMRUNNER_H
