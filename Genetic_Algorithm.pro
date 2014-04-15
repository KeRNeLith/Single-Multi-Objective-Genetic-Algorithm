TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

INCLUDEPATH +=  ./include

SOURCES += example/main.cpp \
    src/ga.cpp \
    src/chromosome.cpp \
    src/population.cpp \
    src/singleobjectivega.cpp \
    example/roulettewheel.cpp \
    example/chromosomeintint.cpp

HEADERS += \
    include/ga.h \
    include/chromosome.h \
    include/population.h \
    include/General.h \
    include/singleobjectivega.h \
    example/roulettewheel.h \
    example/chromosomeintint.h

