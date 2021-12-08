#-------------------------------------------------
#
# Project created by QtCreator 2017-05-19T17:20:57
#
#-------------------------------------------------

QT       -= core gui

TARGET = ravstorage
TEMPLATE = lib
CONFIG += plugin

DEFINES += STORAGE_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += storage.cpp \
    cinifile.cpp \
    Entry/settingentry.cpp \
    SQLite3/sqlite3.c \
    Entry/baseentry.cpp \
    CPPSQLite/cppsqlite3.cpp \
    Entry/virusinfoentry.cpp \
    Entry/logevententry.cpp \
    Entry/scheduleentry.cpp \
    Entry/whitelistentry.cpp \
    Entry/commeventlog.cpp \
    Entry/quarantinelog.cpp \
    Entry/netaccessentry.cpp \
    Entry/urlruleentry.cpp \
    Entry/sourcelistentry.cpp

HEADERS += storage.h\
        storage_global.h \
    cinifile.h \
    Entry/settingentry.h \
    SQLite3/sqlite3.h \
    Entry/baseentry.h \
    CPPSQLite/cppsqlite3.h \
    Entry/virusinfoentry.h \
    Entry/logevententry.h \
    Entry/scheduleentry.h \
    Entry/whitelistentry.h \
    Entry/commeventlog.h \
    Entry/quarantinelog.h \
    Entry/netaccessentry.h \
    Entry/urlruleentry.h \
    Entry/commonentry.h \
    Entry/sourcelistentry.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

INCLUDEPATH += "../CommonAPI"
INCLUDEPATH += "./SQLite3"

DISTFILES += \
    HiberLite/readme.txt
