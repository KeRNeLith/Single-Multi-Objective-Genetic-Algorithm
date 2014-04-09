TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

SOURCES += example/main.cpp \
    src/ga.cpp \
    src/chromosome.cpp \
    src/population.cpp \
    example/roulettewheel.cpp \
    example/chromosomeintint.cpp

HEADERS += \
    include/ga.h \
    include/chromosome.h \
    include/population.h \
    example/roulettewheel.h \
    include/General.h \
    example/chromosomeintint.h

