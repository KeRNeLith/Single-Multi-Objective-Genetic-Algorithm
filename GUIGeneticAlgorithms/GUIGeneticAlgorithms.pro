#-------------------------------------------------
#
# Project created by QtCreator 2014-04-24T16:02:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GUIGeneticAlgorithms
TEMPLATE = app
CONFIG += c++11

DESTDIR = $$_PRO_FILE_PWD_/build

INCLUDEPATH +=  ./include \
                ../GA-NSGAII/include \
                ../GA-NSGAII/example

SOURCES +=  ./src/main.cpp \
            ./src/mainwindow.cpp \
            ./src/paramsdockwidget.cpp \
            ../GA-NSGAII/example/chromosomeintint.cpp \
            ../GA-NSGAII/example/chromosomemintint.cpp \
            ./src/solutionlisterdockwidget.cpp

HEADERS  += ./include/mainwindow.h \
            ./include/paramsdockwidget.h \
            ../GA-NSGAII/example/chromosomeintint.h \
            ../GA-NSGAII/example/chromosomemintint.h \
            ./include/solutionlisterdockwidget.h

FORMS    += ./gui/mainwindow.ui \
            ./gui/paramsdockwidget.ui \
            ./gui/solutionlisterdockwidget.ui
