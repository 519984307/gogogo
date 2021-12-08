#-------------------------------------------------
#
# Project created by QtCreator 2017-11-29T16:57:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = config
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
#DEFINES += COMM_WITH_DBUS
DEFINES += COMMON_STRING_API COMMON_HASH_API COMMON_OS_API COMMON_PATH_API COMMON_JSON_API COMMON_LOG_API COMMON_ZIP_API QT_NO_VERSION_TAGGING
#RAVQT_FUNC_FLAG  0x01:original 0x02:whitelist 0x04:kv-backup 0x08:v-scan 0x10:schedul-scan 0x20:file-monitor 0x40:udisk-monitor
DEFINES += RAVQT_FUNC_FLAG=127

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    main.cpp \
    mainwindow.cpp \
    topwidget.cpp \
    leftwidget.cpp \
    origroup.cpp \
    whitelistgroup.cpp \
    kvgroup.cpp \
    filegroup.cpp \
    udiskgroup.cpp \
    systemgroup.cpp \
    admingroup.cpp \
    traygroup.cpp \
    licensegroup.cpp \
    upgradegroup.cpp \
    appgroup.cpp \
    topscrollwidget.cpp \
    endscrollwidget.cpp \
    backupgroup.cpp \
    righttopwidget.cpp \
    rightendwidget.cpp \
    timergroup.cpp \
    netblacklistgroup.cpp \
    midscrollwidget.cpp \
    rightmidwidget.cpp \
    common/pushbutton.cpp \
    common/imgtxtlabel.cpp \
    common/globalconf.cpp \
    common/checkboxlabel.cpp \
    common/filedirbutton.cpp \
    common/dirfiletable.cpp \
    common/extnametable.cpp \
    common/dirselectdialog.cpp \
    common/radiobutton.cpp \
    common/clickedlabel.cpp \
    common/imgbutton.cpp \
    common/combobox.cpp \
    common/weekdayselector.cpp \
    common/blacklisturltable.cpp \
    common/blacklistiptable.cpp \
    common/dirtable.cpp \
    common/procnametable.cpp \
    ../../BaseController/application.cpp \
    ../../BaseController/storagebridge.cpp \
    ../../BaseController/appconfig.cpp \
    ../../CommonAPI/centerlistener.cpp \
    operationpower.cpp \
    common/updatesourcetable.cpp \
    common/privatecloudtable.cpp \
    centerdiscoverygroup.cpp \
    common/centerdiscoverytable.cpp

HEADERS += \
    mainwindow.h \
    topwidget.h \
    leftwidget.h \
    origroup.h \
    whitelistgroup.h \
    backupgroup.h \
    kvgroup.h \
    filegroup.h \
    udiskgroup.h \
    systemgroup.h \
    admingroup.h \
    traygroup.h \
    licensegroup.h \
    upgradegroup.h \
    appgroup.h \
    topscrollwidget.h \
    whitelistgroup.h \
    endscrollwidget.h \
    righttopwidget.h \
    rightendwidget.h \
    timergroup.h \
    netblacklistgroup.h \
    midscrollwidget.h \
    rightmidwidget.h \
    common/pushbutton.h \
    common/pubdef.h \
    common/imgtxtlabel.h \
    common/globalconf.h \
    common/checkboxlabel.h \
    common/filedirbutton.h \
    common/dirfiletable.h \
    common/extnametable.h \
    common/dirselectdialog.h \
    common/radiobutton.h \
    common/clickedlabel.h \
    common/imgbutton.h \
    common/combobox.h \
    common/weekdayselector.h \
    common/blacklisturltable.h \
    common/blacklistiptable.h \
    common/dirtable.h \
    common/procnametable.h \
    ../../BaseController/application.h \
    ../../BaseController/storagebridge.h \
    ../../BaseController/appconfig.h \
    ../../CommonAPI/centerlistener.h \
    operationpower.h \
    common/updatesourcetable.h \
    common/privatecloudtable.h \
    centerdiscoverygroup.h \
    common/centerdiscoverytable.h

FORMS += \
        mainwindow.ui

INCLUDEPATH += ./
INCLUDEPATH += common/
INCLUDEPATH += ../../ScanEngine/
INCLUDEPATH += ../../BaseController/
INCLUDEPATH += ../../RavConsole/
INCLUDEPATH += ../../CommonAPI
INCLUDEPATH += ../../Storage
INCLUDEPATH += ../../ThirdPart/include
INCLUDEPATH += ../../Storage/
INCLUDEPATH += ../../Upgrade


#LIBS += -L../build-Storage-Debug -lravstorage
#LIBS += -L../build-CommonAPI-Debug -lravcommon


LIBS += -L/home/rising/workspace/esm_dev/linux/build/3.0.0.1/lib -lravstorage -lravcrypto -lravcommon
#LIBS += -L/opt/BDFZ/RAV/lib -lravstorage -lravcrypto -lravcommon #-lravz
#LIBS += -L/opt/Qt5.9.2/5.9.2/gcc_64/libRESOURCES

RESOURCES += resource.qrc


