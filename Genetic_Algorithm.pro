TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    ga.cpp \
    chromosome.cpp \
    population.cpp

HEADERS += \
    ga.h \
    chromosome.h \
    population.h

