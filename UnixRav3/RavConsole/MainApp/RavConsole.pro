QT += core dbus
QT -= gui

TARGET = RAVRJJH0
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    view.cpp \
    textview.cpp \
    window.cpp \
    screen.cpp \
    menu.cpp \
    titleview.cpp \
    dialog.cpp \
    listview.cpp \
    settingview.cpp \
    editview.cpp \
    optionview.cpp \
    aboutview.cpp \
    ../../BaseController/application.cpp \
    ../../BaseController/framework.cpp \
    ../../CommonAPI/centerlistener.cpp


# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += COMM_WITH_DBUS
DEFINES += COMM_WITH_DBUS
DEFINES += COMMON_STRING_API
DEFINES += COMMON_HASH_API
DEFINES += COMMON_OS_API
DEFINES += COMMON_PATH_API
DEFINES += COMMON_JSON_API
DEFINES += COMMON_LOG_API
DEFINES += COMMON_ZIP_API

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DISTFILES += \
    RavConsole.pro.user \
    RavConsole.pro.user.5f13571 \
    Makefile

HEADERS += \
    config.h \
    view.h \
    textview.h \
    window.h \
    screen.h \
    menu.h \
    titleview.h \
    dialog.h \
    resources.h \
    controller.h \
    logreportview.hpp \
    listview.h \
    logdatasupporter.h \
    settingview.h \
    editview.h \
    optionview.h \
    aboutview.h \
    string_english.h \
    string_chinese.h \
    ../../BaseController/application.h \
    ../../BaseController/framework.h \
    ../../CommonAPI/centerlistener.h


INCLUDEPATH += "../../BaseController"
INCLUDEPATH += "../../CommonAPI"
INCLUDEPATH += "../../Storage"
INCLUDEPATH += "../../Upgrade"
INCLUDEPATH += "../../ScanEngine"
INCLUDEPATH += "../../ThirdPart/include"


LIBS += -L../../build-Storage-Debug -lravstorage
LIBS += -L../../build-CommonAPI-Debug -lravcommon
LIBS += -L../../ThirdPart/lib -lravcrypto -lravcurl -lravssl -lravz -ldbus-1 -lravncurses


#DEFINES += CHINESE
