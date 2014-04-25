#ifndef PARAMSDOCKWIDGET_H
#define PARAMSDOCKWIDGET_H

#include <QDockWidget>

namespace Ui {
class ParamsDockWidget;
}

class ParamsDockWidget
        : public QDockWidget
{
    Q_OBJECT

public:
    explicit ParamsDockWidget(QWidget *parent = 0);
    ~ParamsDockWidget();

    bool getReadParamsFromFileState() const;
    int getNbGenerationsWanted() const;
    int getNbMaxChromosomes() const;
    int getNbGenes() const;
    double getMutateProbability() const;
    double getCrossoverProbability() const;

private:
    Ui::ParamsDockWidget *ui;
};

#endif // PARAMSDOCKWIDGET_H
