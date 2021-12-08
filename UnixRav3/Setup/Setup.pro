#-------------------------------------------------
#
# Project created by QtCreator 2018-02-06T16:27:15
#
#-------------------------------------------------

QT       -= core gui

TARGET = Setup
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle

DEFINES += SETUP_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += setup.cpp \
    devicemonitor.cpp \
    ../RavConsole/Setup/ravcmdsetup.cpp \
    ../Storage/cinifile.cpp

HEADERS += setup.h\
        setup_global.h \
    devicemonitor.h \
    ../Storage/cinifile.h

INCLUDEPATH += "../BaseController"
INCLUDEPATH += "../CommonAPI"
INCLUDEPATH += "../Storage"

LIBS += -L../build-CommonAPI-Debug -lravcommon
LIBS += -L../ThirdPart/lib -lravcrypto  -lravz

unix {
    target.path = /usr/lib
    INSTALLS += target
}
