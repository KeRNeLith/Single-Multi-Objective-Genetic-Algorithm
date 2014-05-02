#ifndef PARETOOPTIMALFRONTWIDGET_H
#define PARETOOPTIMALFRONTWIDGET_H

#include <QWidget>

class ParetoOptimalFrontWidget
        : public QWidget
{
    Q_OBJECT

public:
    explicit ParetoOptimalFrontWidget(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent* event);

signals:

public slots:

};

#endif // PARETOOPTIMALFRONTWIDGET_H
