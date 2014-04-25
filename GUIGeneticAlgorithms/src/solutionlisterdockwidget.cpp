#include "solutionlisterdockwidget.h"
#include "ui_solutionlisterdockwidget.h"

SolutionListerDockWidget::SolutionListerDockWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::SolutionListerDockWidget)
{
    ui->setupUi(this);
}

SolutionListerDockWidget::~SolutionListerDockWidget()
{
    delete ui;
}

void SolutionListerDockWidget::clearSolutionList()
{
    ui->listSolutionsWidget->clear();
    repaint();
}

void SolutionListerDockWidget::setSolutionList(const std::vector<QString> solutionsList)
{
    for (auto it = solutionsList.begin() ; it != solutionsList.end() ; it++)
        ui->listSolutionsWidget->addItem(*it);
}
