QT += core dbus
QT -= gui

TARGET = RAVGGFW0
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += COMM_WITH_DBUS
DEFINES += COMMON_STRING_API
DEFINES += COMMON_HASH_API
DEFINES += COMMON_OS_API
DEFINES += COMMON_PATH_API
DEFINES += COMMON_JSON_API
DEFINES += COMMON_LOG_API
DEFINES += COMMON_ZIP_API
#DEFINES += QT_NO_VERSION_TAGGING

QMAKE_CXXFLAGS += -DQT_COMPILING_QSTRING_COMPAT_CPP

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    mainengine.cpp \
    clientinfo.cpp \
    clientinfo_linux.cpp \
    scheduleengine.cpp \
    webserver.cpp \
    taskproxy.cpp \
    ukcom_u.c \
    usbpluginchecker.cpp \
    defenmodproxy.cpp \
    policysyncparser.cpp \
    scannotifycallback.cpp \
    pluginentry.cpp \
    ../BaseController/framework.cpp \
    ../BaseController/scanproxy.cpp \
    ../BaseController/controller.cpp \
    ../BaseController/application.cpp \
    ../CommonAPI/centerlistener.cpp


INCLUDEPATH += "../Communicator/"
INCLUDEPATH += "../ScanEngine/"
INCLUDEPATH += "../CommonAPI"
INCLUDEPATH += "../Storage"
INCLUDEPATH += "../BaseController"
INCLUDEPATH += "../Upgrade"
INCLUDEPATH += "../unix_engine2013"
INCLUDEPATH += "../ThirdPart/include/libxml2"

LIBS += -L../build-Storage-Debug -lravstorage
LIBS += -L../build-CommonAPI-Debug -lravcommon
LIBS += -L../build-ScanEngine-Debug -lravscanengine
LIBS += -L../build-Communicator-Debug -lravcommunicator
LIBS += -L../build-Upgrade-Debug -lravupdate

#LIBS += -L../lib -lravstorage -lravcommon -lravscanengine -lravcommunicator -lravupdate

LIBS += -L../ThirdPart/lib -lravcrypto -lravcurl -lravssl -lravz -lravxml2
LIBS += -L../unix_engine2013/rame.bin -lrxruntim -lrxmss -lrxcoml -lrxffr -lrxsig64
LIBS += -ludev
#LIBS += /home/miwoo/workspace/qt/Qt5.8.0/5.8/gcc_64/lib/libQt5Core.so

HEADERS += \
    mainengine.h \
    clientinfo.h \
    scheduleengine.h \
    webserver.h \
    taskproxy.h \
    ukcom.h \
    usbpluginchecker.h \
    defenmodproxy.h \
    panzer_if.h \
    policysyncparser.h \
    scannotifycallback.h \
    ../BaseController/framework.h \
    ../BaseController/scanproxy.h \
    ../BaseController/controller.h \
    ../BaseController/application.h \
    ../CommonAPI/centerlistener.h
