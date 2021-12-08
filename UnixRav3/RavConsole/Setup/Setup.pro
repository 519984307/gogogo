QT -= core
QT -= gui

TARGET = Setup
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    ravcmdsetup.cpp \
    ../../Setup/setup.cpp \
    ../../Storage/cinifile.cpp \
    ../../CommonAPI/commonapi.cpp \
    ../../CommonAPI/crypt/md5.cpp \
    ../../CommonAPI/configfile.cpp \
    ../../CommonAPI/zip/unixzip.cpp \
    ../../CommonAPI/crypt/base64.cpp \
    ../../CommonAPI/crypt/rc4.cpp

INCLUDEPATH +=  "../../Setup"

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += COMM_WITH_DBUS
DEFINES += COMMON_STRING_API COMMON_HASH_API COMMON_OS_API COMMON_PATH_API COMMON_ZIP_API WITHOUT_CRYPTO

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    ../../Setup/setup.h \
    ../../Setup/setup_global.h \
    ../../CommonAPI/commonosapi.h \
    ../../CommonAPI/commonpathapi.h \
    ../../CommonAPI/commonstringapi.h \
    ../../Storage/cinifile.h \
    ../../CommonAPI/crypt/md5.h \
    ../../CommonAPI/configfile.h \
    ../../CommonAPI/zip/unixzip.h \
    ../../CommonAPI/crypt/base64.h \
    ../../CommonAPI/crypt/rc4.h

INCLUDEPATH += ../../BaseController
INCLUDEPATH += ../../Setup
INCLUDEPATH += ../../CommonAPI
INCLUDEPATH += ../../Storage

LIBS += ../../ThirdPart/lib/libravz.a

