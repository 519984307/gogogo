#-------------------------------------------------
#
# Project created by QtCreator 2017-05-19T17:17:21
#
#-------------------------------------------------

QT       -= core gui

TARGET = ravscanengine
TEMPLATE = lib
CONFIG += plugin

DEFINES += SCANENGINE_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += PLATFORM_TYPE_LINUX_INTEL

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += scanengine.cpp \
    multivirusengine.cpp \
    utils.cpp \
    ravquafile.cpp \
    ravlockfile.cpp \
    scanthreadvari.cpp \
    countfilevari.cpp \
    get_cpu.c

HEADERS += scanengine.h\
        scanengine_global.h \
    enginecallback.h \
    multivirusengine.h \
    utils.h \
    ravquafile.h \
    ravquadefined.h \
    ravlockfile.h \
    scanconfig.h \
    engineobserver.h \
    scanthreadvari.h \
    countfilevari.h \
    get_cpu.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

LIBS += -L../unix_engine2013/rame.bin -lrxruntim -lrxmss -lrxcoml -lrxffr -lrxsig64
INCLUDEPATH += "../unix_engine2013"

QMAKE_CFLAGS += "-Wunused-variable"
