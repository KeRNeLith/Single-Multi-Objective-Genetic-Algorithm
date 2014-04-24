#-------------------------------------------------
#
# Project created by QtCreator 2014-04-24T16:02:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GUIGeneticAlgorithms
TEMPLATE = app

DESTDIR = $$_PRO_FILE_PWD_/build

SOURCES +=  ./src/main.cpp \
            ./src/mainwindow.cpp

HEADERS  += ./include/mainwindow.h

FORMS    += ./gui/mainwindow.ui
