#include "paramsdockwidget.h"
#include "ui_paramsdockwidget.h"

ParamsDockWidget::ParamsDockWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::ParamsDockWidget)
{
    ui->setupUi(this);
}

ParamsDockWidget::~ParamsDockWidget()
{
    delete ui;
}

bool ParamsDockWidget::getReadParamsFromFileState() const
{
    return ui->readFileCheckBox->isChecked();
}

void ParamsDockWidget::setReadParamsFromFileState(bool state)
{
    ui->readFileCheckBox->setChecked(state);
}

int ParamsDockWidget::getNbGenerationsWanted() const
{
    return ui->nbGenerationsWantedSpinBox->value();
}

int ParamsDockWidget::getNbMaxChromosomes() const
{
    return ui->nbMaxChromosomesSpinBox->value();
}

int ParamsDockWidget::getNbGenes() const
{
    return ui->nbGenesLabelSpinBox->value();
}

double ParamsDockWidget::getMutateProbability() const
{
    return ui->mutateProbabilitySpinBox->value();
}

double ParamsDockWidget::getCrossoverProbability() const
{
    return ui->crossoverProbabilitySpinBox->value();
}
