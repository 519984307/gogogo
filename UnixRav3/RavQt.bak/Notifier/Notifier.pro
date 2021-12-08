#-------------------------------------------------
#
# Project created by QtCreator 2018-07-02T13:35:00
#
#-------------------------------------------------

QT       += core gui dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RAVGJCZ0
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += COMM_WITH_DBUS
DEFINES += QT_NO_VERSION_TAGGING

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
    notifycontroller.cpp \
    popscreen.cpp \
    pushbutton.cpp \
    screenwidget.cpp \
    ../../CommonAPI/centerlistener.cpp

HEADERS  += notifycontroller.h \
    popscreen.h \
    pushbutton.h \
    screenwidget.h \
    ../../CommonAPI/centerlistener.h

INCLUDEPATH += "../../CommonAPI"
INCLUDEPATH += "../../BaseController"

LIBS += -L../../build-CommonAPI-Debug -lravcommon
#LIBS += -L../../lib -lravcommon
LIBS += -L../../ThirdPart/lib -lravcrypto -lravz

RESOURCES += \
    resource.qrc

