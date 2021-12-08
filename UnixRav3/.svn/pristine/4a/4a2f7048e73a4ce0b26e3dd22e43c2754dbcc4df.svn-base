#-------------------------------------------------
#
# Project created by QtCreator 2018-01-22T15:01:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LogViewer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
#DEFINES += COMM_WITH_DBUS
DEFINES += COMMON_STRING_API COMMON_HASH_API COMMON_OS_API COMMON_PATH_API COMMON_JSON_API COMMON_LOG_API COMMON_ZIP_API

#RAVQT_FUNC_FLAG  0x01:original 0x02:whitelist 0x04:kv-backup 0x08:v-scan 0x10:schedul-scan 0x20:file-monitor 0x40:udisk-monitor
DEFINES += RAVQT_FUNC_FLAG=127

greaterThan(QT_MINOR_VERSION, 4): DEFINES += QT_NEW_HEADER_54

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    main.cpp \
    mainwindow.cpp \
    leftwidget.cpp \
    topwidget.cpp \
    common/clickedlabel.cpp \
    common/combobox.cpp \
    common/dirfiletable.cpp \
    common/dirselectdialog.cpp \
    common/extnametable.cpp \
    common/filedirbutton.cpp \
    common/globalconf.cpp \
    common/imgbutton.cpp \
    common/imgtxtlabel.cpp \
    common/pushbutton.cpp \
    common/radiobutton.cpp \
    common/sysmaxbtn.cpp \
    common/checkboxlabel.cpp \
    common/searchedit.cpp \
    virusdetailgroup.cpp \
    scaneventgroup.cpp \
    sysreinforcegroup.cpp \
    appreinforcegroup.cpp \
    isolationgroup.cpp \
    rightwidget.cpp \
    sysloggroup.cpp \
    installgroup.cpp \
    urlaccessgroup.cpp \
    iprulegroup.cpp \
    ../../BaseController/application.cpp \
    ../../BaseController/appconfig.cpp \
    ../../BaseController/storagebridge.cpp \
    ../../CommonAPI/centerlistener.cpp \
    operationpower.cpp \
    confirmrestore.cpp \
    common/imagebutton.cpp

HEADERS += \
    mainwindow.h \
    leftwidget.h \
    topwidget.h \
    urlaccessgroup.h \
    iprulegroup.h \
    common/clickedlabel.h \
    common/combobox.h \
    common/dirfiletable.h \
    common/dirselectdialog.h \
    common/extnametable.h \
    common/filedirbutton.h \
    common/globalconf.h \
    common/imgbutton.h \
    common/imgtxtlabel.h \
    common/pubdef.h \
    common/pushbutton.h \
    common/radiobutton.h \
    common/sysmaxbtn.h \
    common/checkboxlabel.h \
    common/searchedit.h \
    virusdetailgroup.h \
    scaneventgroup.h \
    sysreinforcegroup.h \
    appreinforcegroup.h \
    isolationgroup.h \
    rightwidget.h \
    sysloggroup.h \
    installgroup.h \
    ../../BaseController/application.h \
    ../../BaseController/appconfig.h \
    ../../BaseController/storagebridge.h \
    ../../CommonAPI/centerlistener.h \
    operationpower.h \
    confirmrestore.h \
    common/imagebutton.h


RESOURCES += \
    resource.qrc

INCLUDEPATH += ./
INCLUDEPATH += common/
INCLUDEPATH += ../../ScanEngine/
INCLUDEPATH += ../../BaseController/
INCLUDEPATH += ../../RavConsole/
INCLUDEPATH += ../../CommonAPI
INCLUDEPATH += ../../Storage
INCLUDEPATH += ../../Upgrade
INCLUDEPATH += ../../ThirdPart/include


#LIBS += -L../build-Storage-Debug -lravstorage
#LIBS += -L../build-CommonAPI-Debug -lravcommon
#LIBS += -L../ThirdPart/lib -lravcrypto -lravz
LIBS += -L/opt/Qt5.9.2/5.9.2/gcc_64/lib
LIBS += -L/home/rising/workspace/esm_dev/linux/build/3.0.0.1/lib
LIBS += -L../ThirdPart/lib
LIBS += -L/opt/BDFZ/RAV/lib -lravstorage -lravcommon -lravcrypto -lravz
