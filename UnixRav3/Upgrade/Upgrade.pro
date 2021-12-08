#-------------------------------------------------
#
# Project created by QtCreator 2018-02-06T16:26:25
#
#-------------------------------------------------

QT -= core
QT -= gui

TARGET = ravupdate
TEMPLATE = lib
CONFIG += plugin

#TARGET = ravupdate
#TEMPLATE = app

DEFINES += UPGRADE_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += COMMON_STRING_API COMMON_HASH_API COMMON_OS_API COMMON_PATH_API COMMON_JSON_API COMMON_LOG_API COMMON_ZIP_API

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += UpdateConsole/main.cpp \
    Common/updateapp.cpp \
    Common/upgrade.cpp \
    Common/finishapp.cpp \
    ../Storage/cinifile.cpp


HEADERS += \
    Common/updateapp.h \
    Common/updateconfig.h \
    Common/updateobserver.h \
    Common/upgrade.h \
    Common/finishapp.h \
    ../Storage/cinifile.h

#SOURCES += UpdateConsole/main.cpp Common/finishapp.cpp \
#    ../Storage/cinifile.cpp
#HEADERS += Common/finishapp.h \
#    ../Storage/cinifile.h

INCLUDEPATH += "Common"
INCLUDEPATH += "../BaseController"
INCLUDEPATH += "../ScanEngine"
INCLUDEPATH += "../CommonAPI"
INCLUDEPATH += "../Communicator"
INCLUDEPATH += "../ThirdPart/include"
INCLUDEPATH += "../ThirdPart/include/libxml2"
INCLUDEPATH += "../unix_engine2013"
INCLUDEPATH += "../Storage"

LIBS += -L../build-CommonAPI-Desktop-Debug -lravcommon
LIBS += -L../build-ScanEngine-Desktop-Debug -lravscanengine
LIBS += -L../ThirdPart/lib -lravcrypto -lravcurl -lravssl -lravxml2 -lravz
LIBS += -L../unix_engine2013/rame.bin -lrxruntim -lrxmss -lrxcoml -lrxffr -lrxsig64

unix {
    target.path = /usr/lib
    INSTALLS += target
}
