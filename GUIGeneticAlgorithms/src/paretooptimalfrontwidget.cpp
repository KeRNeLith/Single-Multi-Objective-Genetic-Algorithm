#include "paretooptimalfrontwidget.h"

ParetoOptimalFrontWidget::ParetoOptimalFrontWidget(QWidget *parent) :
    QWidget(parent)
{
}

void ParetoOptimalFrontWidget::loadFile(const char* fileName)
{
    m_coordinates.clear();

    std::ifstream file(fileName, std::ios::in);  // Open file

    if(file)  // Open Success
    {
        int i = 0;
        while (!file.eof())
        {
            i++;
            std::string line;           // One line of the file
            std::getline(file, line);   // Recuperate one line

            std::vector<std::string> lineSplited = split(line, '|');    // Split the line on char '|'

            if (lineSplited.size() < 4) // Error
                throw std::runtime_error("File is corrupted : Invalid Number of datas.");

            std::vector<std::string> fitnessSubLineSplited = split(lineSplited[3], ':');    // Split the line on char ':'

            if (fitnessSubLineSplited.size() < 2)   // Error
                throw std::runtime_error("File is corrupted : No Fitness values.");

            std::vector<std::string> fitnessValue = split(fitnessSubLineSplited[1], '/');    // Split the line on char '/'
            for (unsigned int i = 0 ; i  < fitnessValue.size() ; i++)
                fitnessValue[i] = trim(fitnessValue[i]);

            std::vector<double> coordinates;
            for (unsigned int i = 0 ; i  < fitnessValue.size()-1 ; i++)
                coordinates.push_back(getNumber<double>(fitnessValue[i]));
            m_coordinates.push_back(coordinates);
        }

        file.close();  // Close file
    }
    else
        throw std::runtime_error("Impossible to open file or file doesn't exist!");
}

void ParetoOptimalFrontWidget::paintEvent(QPaintEvent* event)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    int min = width() < height() ? width() : height();
    p.setViewport((width()-min)/2, (height()-min)/2, min, min);
    p.setWindow(-400, -400, 800, 800);

    QWidget::paintEvent(event);
}
