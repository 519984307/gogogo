QT -= core
QT -= gui

CONFIG -= c++11

TARGET = ContextMenu
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = lib

SOURCES += \
    caja-rav.c \
    rav-module.c

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    caja-rav.h \
    config.h

INCLUDEPATH += ../CommonAPI
INCLUDEPATH += ../BaseController
INCLUDEPATH += /usr/include/caja
INCLUDEPATH += /usr/include/glib-2.0
INCLUDEPATH += /usr/lib/x86_64-linux-gnu/glib-2.0/include
INCLUDEPATH += /usr/include/gtk-2.0
INCLUDEPATH += /usr/include/cairo
INCLUDEPATH += /usr/include/pango-1.0
INCLUDEPATH += /usr/lib/x86_64-linux-gnu/gtk-2.0/include
INCLUDEPATH += /usr/include/gdk-pixbuf-2.0
INCLUDEPATH += /usr/include/atk-1.0

LIBS += -lcaja-extension -lgtk-x11-2.0 -lgdk-x11-2.0 -latk-1.0 -lpangoft2-1.0 -lpangocairo-1.0 -lgdk_pixbuf-2.0 -lcairo -lpango-1.0 -lfontconfig -lfreetype -lgio-2.0 -lgobject-2.0 -lglib-2.0
