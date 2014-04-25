#ifndef SOLUTIONLISTERDOCKWIDGET_H
#define SOLUTIONLISTERDOCKWIDGET_H

#include <vector>

#include <QDockWidget>

namespace Ui {
class SolutionListerDockWidget;
}

class SolutionListerDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit SolutionListerDockWidget(QWidget *parent = 0);
    ~SolutionListerDockWidget();

    void clearSolutionList();

    void setSolutionList(const std::vector<QString> solutionsList);

private:
    Ui::SolutionListerDockWidget *ui;
};

#endif // SOLUTIONLISTERDOCKWIDGET_H
