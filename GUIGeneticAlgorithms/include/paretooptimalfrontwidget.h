#ifndef PARETOOPTIMALFRONTWIDGET_H
#define PARETOOPTIMALFRONTWIDGET_H

#include <fstream>

#include <QWidget>
#include <QPainter>
#include <QKeyEvent>

#include "General.h"
#include "AlgoString.h"

class ParetoOptimalFrontWidget
        : public QWidget
{
    Q_OBJECT

private:
    std::string m_currentLoadedGraph;
    std::vector< std::vector<double> > m_coordinates;

    int getLoadedGraphId();
    bool fileExist(const std::string& fileName);

public:
    explicit ParetoOptimalFrontWidget(QWidget *parent = 0);

    void loadFile(const char* fileName);
    void loadFile(const std::string& fileName) { loadFile(fileName.c_str()); }

    void draw(QPaintDevice* device);

protected:
    void paintEvent(QPaintEvent* event);
    void keyPressEvent(QKeyEvent* event);

signals:

public slots:

};

#endif // PARETOOPTIMALFRONTWIDGET_H
