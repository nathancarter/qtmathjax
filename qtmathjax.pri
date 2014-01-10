
QT += core gui webkitwidgets svg
greaterThan( QT_MAJOR_VERSION, 4 ): QT += widgets

DEPENDPATH += $$PWD
INCLUDEPATH += $$PWD
VPATH += $$PWD

SOURCES += $$PWD/texengine.cpp
HEADERS += $$PWD/texengine.h
RESOURCES += $$PWD/qtmathjax.qrc

