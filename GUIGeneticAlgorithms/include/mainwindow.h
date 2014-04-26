#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vector>

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QThread>
#include <QMetaType>

#include "paramsdockwidget.h"
#include "solutionlisterdockwidget.h"
#include "algorithmrunner.h"

#include "ui_mainwindow.h"

typedef std::vector<QString> StringVector;

namespace Ui {
class MainWindow;
}

class MainWindow
        : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;

    ParamsDockWidget* m_paramsDW;
    SolutionListerDockWidget* m_solutionsDW;

    QString m_paramsFileName;

    QThread m_mainWindowThread;

    void changePushButtonState(bool state = true);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QString getParamsFileName() { return m_paramsFileName; }
    Ui::MainWindow* getUI() { return ui; }
    ParamsDockWidget* getParamsDockWidget() { return m_paramsDW; }

public slots:
    void openParamsFile();
    void runGAAlgorithm();
    void runNSGA2Algorithm();
    void handleResults(const std::vector<QString>& result);
    void showFileUnknownMessage();
    void showAlgorithmFailureMessage(const char* message);
    void updateProgressBarValue(int value);

    void about();

signals:
    void launchAlgorithm(const QString& parameters);
};

#endif // MAINWINDOW_H
