#-------------------------------------------------
#
# Project created by QtCreator 2017-12-14T05:34:10
#
#-------------------------------------------------

QT       -= core gui

TARGET = ravcommon
TEMPLATE = lib

CONFIG += plugin

DEFINES += COMMONAPI_LIBRARY
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
#DEFINES += COMM_WITH_SOCKET
DEFINES += COMM_WITH_DBUS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += commonapi.cpp \
    crypt/base64.cpp \
    crypt/md5.cpp \
    crypt/rc4.cpp \
    json/json_internalarray.inl \
    json/json_internalmap.inl \
    json/json_reader.cpp \
    json/json_value.cpp \
    json/json_valueiterator.inl \
    json/json_writer.cpp \
    handler.cpp \
    communicatebundle.cpp \
    zip/unzip.cpp \
    zip/unixzip.cpp \
    configfile.cpp

HEADERS += commonapi.h\
        commonapi_global.h \
    crypt/base64.h \
    crypt/md5.h \
    crypt/rc4.h \
    json/arraylist.h \
    json/autolink.h \
    json/bits.h \
    json/config.h \
    json/debug.h \
    json/features.h \
    json/forwards.h \
    json/json.h \
    json/json_batchallocator.h \
    json/json_c_version.h \
    json/json_config.h \
    json/json_inttypes.h \
    json/json_object.h \
    json/json_object_iterator.h \
    json/json_object_private.h \
    json/json_tokener.h \
    json/json_util.h \
    json/linkhash.h \
    json/printbuf.h \
    json/random_seed.h \
    json/reader.h \
    json/value.h \
    json/writer.h \
    circlelink.h \
    listdatadelegate.h \
    handler.h \
    rserializer.h \
    commonlocker.h \
    threadpool.h \
    communicatebundle.h \
    zip/unzip.h \
    zip/zip.h \
    zip/unixzip.h \
    configfile.h

INCLUDEPATH += ../ThirdPart/include

LIBS += -L../ThirdPart/lib -lravz -lravcrypto

unix {
    target.path = /usr/lib
    INSTALLS += target
}
