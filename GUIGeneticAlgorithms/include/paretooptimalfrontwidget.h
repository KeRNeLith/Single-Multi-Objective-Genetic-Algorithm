#ifndef PARETOOPTIMALFRONTWIDGET_H
#define PARETOOPTIMALFRONTWIDGET_H

#include <fstream>

#include <QWidget>
#include <QPainter>

#include "General.h"
#include "AlgoString.h"

class ParetoOptimalFrontWidget
        : public QWidget
{
    Q_OBJECT

private:
    std::vector< std::vector<double> > m_coordinates;

public:
    explicit ParetoOptimalFrontWidget(QWidget *parent = 0);

    void loadFile(const char* fileName);
    void loadFile(const std::string& fileName) { loadFile(fileName.c_str()); }

protected:
    void paintEvent(QPaintEvent* event);

signals:

public slots:

};

#endif // PARETOOPTIMALFRONTWIDGET_H
