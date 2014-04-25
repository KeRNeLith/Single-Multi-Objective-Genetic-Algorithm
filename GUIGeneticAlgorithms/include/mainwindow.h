#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>

#include "singleobjectivega.h"
#include "nsgaii.h"

//#include "../GA-NSGAII/example/roulettewheel.h"
//#include "../GA-NSGAII/example/chromosomeintint.h"
//#include "roulettewheel.h"
//#include "chromosomeintint.h"

#include "paramsdockwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow
        : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void openParamsFile();
    void runGAAlgorithm();
    void runNSGA2Algorithm();

    void about();

private:
    Ui::MainWindow *ui;

    ParamsDockWidget* m_paramsDW;

    QString m_paramsFileName;

    void changePushButtonState();
};

#endif // MAINWINDOW_H
