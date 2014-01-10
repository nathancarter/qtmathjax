#-------------------------------------------------
#
# Project created by QtCreator 2014-01-07T15:35:35
#
#-------------------------------------------------

QT       += core gui webkitwidgets svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TeX2SVG
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp\
        texengine.cpp

HEADERS  += mainwindow.h\
        texengine.h

FORMS    += mainwindow.ui

RESOURCES += qtmathjax.qrc
