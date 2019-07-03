#-------------------------------------------------
#
# Project created by QtCreator 2019-07-02T18:35:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = simUI
TEMPLATE = app


SOURCES += main.cpp\
        simui.cpp \
    aboutdialog.cpp

HEADERS  += simui.h \
    aboutdialog.h

FORMS    += simui.ui \
    aboutdialog.ui

RESOURCES += \
    simUIart.qrc
