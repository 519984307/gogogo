TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    upgrademaker.cpp \
    ../CommonAPI/zip/unixzip.cpp \
    ../CommonAPI/crypt/md5.cpp \
    ../CommonAPI/zip/unzip.cpp \
    ravpacksetup.cpp


DISTFILES += \
    mkpkg.sh \
    package.sh

HEADERS += \
    ../CommonAPI/zip/unixzip.h \
    ../CommonAPI/crypt/md5.h \
    ../CommonAPI/zip/unzip.h

INCLUDEPATH += "../CommonAPI/zip"
INCLUDEPATH += "../CommonAPI/crypt"

LIBS += -lz
