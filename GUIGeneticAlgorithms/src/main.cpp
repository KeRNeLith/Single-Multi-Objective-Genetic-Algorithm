#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("NSGA-II and Genetic Algorithm Demo");
    a.setApplicationVersion("0.1.0");

    MainWindow w;
    w.show();

    return a.exec();
}
