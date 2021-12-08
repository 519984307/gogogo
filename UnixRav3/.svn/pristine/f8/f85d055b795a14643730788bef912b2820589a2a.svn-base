#-------------------------------------------------
#
# Project created by QtCreator 2018-02-22T22:43:57
#
#-------------------------------------------------

QT       -= core gui

TARGET = ravbase
TEMPLATE = lib

DEFINES += BASECONTROLLER_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += COMM_WITH_SOCKET

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    application.cpp \
    controller.cpp \
    framework.cpp \
    scanproxy.cpp

HEADERS += \
    application.h \
    controller.h \
    baseconfig.h \
    framework.h \
    scanproxy.h


INCLUDEPATH += "../CommonAPI"
INCLUDEPATH += "../Upgrade"
INCLUDEPATH += "../Storage"
INCLUDEPATH += "../ScanEngine"
INCLUDEPATH += "../unix_engine2013"
INCLUDEPATH += "../ThirdPart/include/dbus-1.0"
INCLUDEPATH += "../ThirdPart/lib/dbus-1.0/include"

LIBS += -L../build-CommonAPI-Debug -lravcommon
LIBS += -L../build-Storage-Debug -lravstorage
LIBS += -L../build-ScanEngine-Debug -lravscanengine
LIBS += -L../ThirdPart/lib -lravsqlite3 -lravcrypto
LIBS += -L../unix_engine2013/rame.bin -lrxruntim -lrxmss -lrxcoml -lrxffr -lrxsig64

unix {
    target.path = /usr/lib
    INSTALLS += target
}
