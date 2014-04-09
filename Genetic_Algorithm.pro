TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

SOURCES += main.cpp \
    ga.cpp \
    chromosome.cpp \
    population.cpp \
    roulettewheel.cpp \
    chromosomeintint.cpp

HEADERS += \
    ga.h \
    chromosome.h \
    population.h \
    roulettewheel.h \
    General.h \
    chromosomeintint.h

