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
    aboutdialog.cpp \
    integration.cpp \
    simoptionsdialog.cpp \
    listbreak.cpp \
    addbreak.cpp \
    tools_registerinspector.cpp \
    tools_runtimedirective.cpp \
    tools_meminspector.cpp \
    tools_specialdebug.cpp

HEADERS  += simui.h \
    aboutdialog.h \
    integration.h \
    simoptionsdialog.h \
    listbreak.h \
    addbreak.h \
    tools_registerinspector.h \
    tools_runtimedirective.h \
    tools_meminspector.h \
    tools_specialdebug.h

FORMS    += simui.ui \
    aboutdialog.ui \
    simoptionsdialog.ui \
    addbreak.ui \
    listbreak.ui \
    tools_registerinspector.ui \
    tools_runtimedirective.ui \
    tools_meminspector.ui \
    tools_specialdebug.ui

RESOURCES += \
    simUIart.qrc \
    simUIlocalization.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../PRISCAS/release/ -lmtcore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../PRISCAS/debug/ -lmtcore

win32:CONFIG(release, debug|release): DEFINES += WIN32

INCLUDEPATH += $$PWD/../include
DEPENDPATH += $$PWD/../include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../PRISCAS/release/libmtcore.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../PRISCAS/debug/libmtcore.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../PRISCAS/release/mtcore.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../PRISCAS/debug/mtcore.lib

unix:!macx: LIBS += -L$$PWD/../build/ -lmtcore

INCLUDEPATH += $$PWD/../include
DEPENDPATH += $$PWD/../include

unix:!macx: PRE_TARGETDEPS += $$PWD/../build/libmtcore.a
