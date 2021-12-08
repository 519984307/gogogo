#-------------------------------------------------
#
# Project created by QtCreator 2017-05-18T10:33:53
#
#-------------------------------------------------

QT       -= core
QT       -= gui

TARGET = ravcommunicator
TEMPLATE = lib
CONFIG += plugin

DEFINES += COMMUNICATOR_LIBRARY
DEFINES += COMM_WITH_DBUS
DEFINES += COMMON_STRING_API
DEFINES += COMMON_HASH_API
DEFINES += COMMON_OS_API
DEFINES += COMMON_PATH_API
DEFINES += COMMON_JSON_API
DEFINES += COMMON_LOG_API
DEFINES += COMMON_ZIP_API

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += DEBUG

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += communicator.cpp \
    models/log/logdatabasic.cpp \
    models/log/logdatalocation.cpp \
    models/log/logdatascanevent.cpp \
    models/log/logdataspammsg.cpp \
    models/log/logdataspamphone.cpp \
    models/log/logdatavirus.cpp \
    models/message/messagemodel.cpp \
    models/policy/commandmodel.cpp \
    models/policy/policymodel.cpp \
    models/contentfactory.cpp \
    net/apiservice.cpp \
    net/httprequest.cpp \
    net/headerfield.cpp \
    models/client/clientinfomodel.cpp \
    models/log/logepstate.cpp \
    models/policy/authorizationmodel.cpp \
    models/log/logcommandstate.cpp

HEADERS += communicator.h\
        communicator_global.h \
    models/modeldefine.h \
    models/log/abstractlogdatamodel.h \
    models/log/logdatabasic.h \
    models/log/logdatalocation.h \
    models/log/logdatascanevent.h \
    models/log/logdataspammsg.h \
    models/log/logdataspamphone.h \
    models/log/logdatavirus.h \
    models/log/reportlogmodel.h \
    models/message/messagemodel.h \
    models/policy/commandmodel.h \
    models/policy/policymodel.h \
    models/contentfactory.h \
    models/contentmodel.h \
    models/messagepackage.h \
    models/messagepartbody.h \
    net/apiservice.h \
    net/httprequest.h \
    net/headerfield.h \
    models/client/clientinfomodel.h \
    models/log/logepstate.h \
    models/policy/authorizationmodel.h \
    models/log/logcommandstate.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

LIBS += -L../build-CommonAPI-Desktop-Debug -lravcommon
LIBS += -L../ThirdPart/lib -lravcrypto -lravcurl -lravssl

INCLUDEPATH += "../ThirdPart/include"
INCLUDEPATH += "../CommonAPI"
