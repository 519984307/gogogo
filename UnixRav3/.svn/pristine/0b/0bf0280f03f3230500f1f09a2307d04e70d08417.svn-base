#-------------------------------------------------
#
# Project created by QtCreator 2019-04-30T15:44:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Uninstall
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += COMMON_STRING_API COMMON_HASH_API COMMON_OS_API COMMON_PATH_API COMMON_ZIP_API WITHOUT_CRYPTO COMMON_JSON_API

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH +=  ../../BaseController
INCLUDEPATH +=  ../../CommonAPI
INCLUDEPATH +=  ../../Communicator
INCLUDEPATH +=  ../../Storage
INCLUDEPATH +=  ../../Setup
INCLUDEPATH +=  ../../unix_engine2013/rame/lib3rd/json/src
INCLUDEPATH +=  ../../ThirdPart/include
INCLUDEPATH +=  ../../ThirdPart/include/libxml2


SOURCES += \
        main.cpp \
        dialog.cpp \
        guiuninstaller.cpp \
        ../../Setup/setup.cpp \
        ../../CommonAPI/configfile.cpp \
        ../../CommonAPI/commonapi.cpp \
        ../../CommonAPI/communicatebundle.cpp \
        ../../BaseController/appconfig.cpp \
        ../../Storage/cinifile.cpp \
        ../../CommonAPI/crypt/md5.cpp \
        ../../CommonAPI/crypt/base64.cpp \
        ../../CommonAPI/crypt/rc4.cpp \
        ../../CommonAPI/zip/unixzip.cpp \
        ../../CommonAPI/json/json_reader.cpp \
        ../../CommonAPI/json/json_writer.cpp \
        ../../CommonAPI/json/json_value.cpp

HEADERS += \
        dialog.h \
        guiuninstaller.h \
        ../../Setup/setup.h \
        ../../CommonAPI/configfile.h \
        ../../CommonAPI/commonapi.h \
        ../../CommonAPI/communicatebundle.h \
        ../../BaseController/appconfig.h \
        ../../Storage/cinifile.h \
        ../../CommonAPI/crypt/md5.h \
        ../../CommonAPI/crypt/base64.h \
        ../../CommonAPI/crypt/rc4.h \
        ../../CommonAPI/zip/unixzip.h

FORMS += \
        dialog.ui

LIBS += -L/opt/Qt5.9.2/5.9.2/gcc_64/lib
LIBS += -L/usr/lib/gcc/x86_64-linux-gnu/6/

LIBS += ../../ThirdPart/lib/libravz.a
LIBS += ../../ThirdPart/lib/libravxml2.a
LIBS += /usr/lib/gcc/x86_64-linux-gnu/6/libstdc++.a



