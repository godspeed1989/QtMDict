
QT += core xml gui widgets

TARGET = QtMDict
TEMPLATE = app
CONFIG += console debug

SOURCES  += main.cpp            \
            mainwindow.cpp      \
            mdictparser.cc      \
            decompress.cc       \
            minilzo/minilzo.c

HEADERS  += mainwindow.h       \
            mdictparser.hh     \
            decompress.hh      \
            minilzo/minilzo.h  \
            minilzo/lzoconf.h  \
            minilzo/lzodefs.h

LIBS     += -lz -liconv
