#include "paretooptimalfrontwidget.h"

ParetoOptimalFrontWidget::ParetoOptimalFrontWidget(QWidget *parent) :
    QWidget(parent)
{
    setMinimumSize(200, 225);
}

void ParetoOptimalFrontWidget::loadFile(const char* fileName)
{
    m_currentLoadedGraph = fileName;
    setWindowTitle(QString(m_currentLoadedGraph.c_str()));
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
    update();
}

void ParetoOptimalFrontWidget::paintEvent(QPaintEvent* event)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    int min = std::min(width(), height());
    p.setViewport((width()-min)/2, (height()-min)/2, min, min);

    if (!m_coordinates.empty()
            && m_coordinates[0].size() < 2)
        return;

    //////////// Search extremes value on X and Y axis ////////////
    // Init axis
    PairMinMax axisX, axisY;
    axisX.index = 0;
    axisX.m_minMax = std::pair<double, double>(m_coordinates[0][0], m_coordinates[0][0]);
    axisY.index = 1;
    axisY.m_minMax = std::pair<double, double>(m_coordinates[0][1], m_coordinates[0][1]);

    // Search
    axisX = std::for_each(m_coordinates.begin(), m_coordinates.end(), axisX);
    axisY = std::for_each(m_coordinates.begin(), m_coordinates.end(), axisY);

    // Determine graph width and height, and keep them to 800 for the window
    double width, height, cWidth, cHeight;
    width = axisX.m_minMax.second - axisX.m_minMax.first;
    height = axisY.m_minMax.second - axisY.m_minMax.first;

    if(width == 0)
        width = 800;
    if(height == 0)
        height = 800;

    double coefficientW = 800/width;
    double coefficientH = 800/height;

    cWidth = width*coefficientW;
    cHeight = height*coefficientH;

    // Window to have a system like
    // 0---> x
    // |
    // \/
    // y
    p.setWindow(0,
                -800,
                cWidth,
                cHeight);

    QPen pen;
    pen.setWidth(1);
    p.setPen(pen);

    //////////// Border and background ////////////
    p.fillRect(0,
               0,
               cWidth,
               -cHeight,
               Qt::white);
    p.drawRect(0,
               0,
               cWidth,
               -cHeight);

    //////////// Draw Axis ////////////
    int nbGraduation = (8/500.0)*min + (2-(8/500.0));
    double incremXGraduation, incremYGraduation;    // use to compute graduation labels
    incremXGraduation = width/(double)nbGraduation;
    incremYGraduation = height/(double)nbGraduation;

    // Text Font
    QFont font;
    font.setPointSizeF(15);
    p.setFont(font);

    QFontMetrics fontMetrics(p.font());
    QString gradutationLabel;
    // Start Graduation Label, will be increase each loop
    double wGrad = axisX.m_minMax.first;
    double hGrad = axisY.m_minMax.first;
    int i = 0;
    while (i < nbGraduation)
    {
        gradutationLabel = QString::number(floorValue(wGrad, 1));
        double graduationX = (i/(double)nbGraduation)*cWidth;
        p.drawLine(QPointF(graduationX, 0), QPointF(graduationX, -4));
        p.drawText(QPointF(graduationX - fontMetrics.width(gradutationLabel)/2, -15), gradutationLabel);

        gradutationLabel = QString::number(floorValue(hGrad, 1));
        double graduationY = (i/(double)nbGraduation)*cHeight;
        p.drawLine(QPointF(0, -graduationY), QPointF(4, -graduationY));
        p.drawText(QPointF(15, -(graduationY - p.font().pointSize()/2)), gradutationLabel);

        i++;
        wGrad += incremXGraduation;
        hGrad += incremYGraduation;
    }

    //////////// Draw Points ////////////
    pen.setColor(Qt::red);
    p.setPen(pen);
    p.setBrush(QBrush(Qt::red));
    for (unsigned int i = 0 ; i < m_coordinates.size() ; i++)
    {
        p.drawEllipse(QPointF(m_coordinates[i][0]*coefficientW, -m_coordinates[i][1]*coefficientH), 5, 5);
    }

    QWidget::paintEvent(event);
}

void ParetoOptimalFrontWidget::keyPressEvent(QKeyEvent* event)
{
    int ID;
    switch (event->key())
    {
    case Qt::Key_Left:
        ID = getLoadedGraphId();
        --ID;
        break;
    case Qt::Key_Right:
        ID = getLoadedGraphId();
        ++ID;
        break;
    default:
        return;
    }

    std::ostringstream fileName;
    fileName << "generation" << ID << ".txt";
    if (fileExist(fileName.str()))
        loadFile(fileName.str());

    QWidget::keyPressEvent(event);
}

int ParetoOptimalFrontWidget::getLoadedGraphId()
{
    if (m_currentLoadedGraph == "")
        return -1;

    return extractNumberFromStr<int>(m_currentLoadedGraph);
}

bool ParetoOptimalFrontWidget::fileExist(const std::string& fileName)
{
    std::ifstream f(fileName.c_str());
    if (f.good())
    {
        f.close();
        return true;
    }
    else
    {
        f.close();
        return false;
    }
}
