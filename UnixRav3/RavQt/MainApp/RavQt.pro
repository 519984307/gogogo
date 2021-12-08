#-------------------------------------------------
#
# Project created by QtCreator 2017-05-08T16:34:20
#
#-------------------------------------------------

QT       += core gui
#dbus

#greaterThan(QT_MAJOR_VERSION, 4):
QT += widgets

TARGET = RavQt
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += CHINESE
DEFINES += COMM_WITH_SOCKET
DEFINES +=
# DEFINES += COMM_WITH_DBUS

#RAVQT_FUNC_FLAG  0x01:original 0x02:whitelist 0x04:kv-backup 0x08:v-scan 0x10:schedul-scan 0x20:file-monitor 0x40:udisk-monitor
DEFINES += RAVQT_FUNC_FLAG=127

DEFINES += QT_NO_VERSION_TAGGING 
DEFINES += COMMON_STRING_API COMMON_HASH_API COMMON_OS_API COMMON_PATH_API COMMON_JSON_API COMMON_LOG_API COMMON_ZIP_API


greaterThan(QT_MINOR_VERSION, 4): DEFINES += QT_NEW_HEADER_54


#DEFINES += INSTALL_PATH=/org/rising/install



# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp \
    home/mainwindow.cpp \
    home/titlewidget.cpp \
    home/bodywidget.cpp \
    home/virus/scanprogresswidget.cpp \
    home/virus/scanresultwidget.cpp \
    home/headerwidget.cpp \
    common/showbutton.cpp \
    common/pushbutton.cpp \
    common/dynmenu.cpp \
    common/animitedbutton.cpp \
    home/topbodywidget.cpp \
    home/midbodywidget.cpp \
    home/botbodywidget.cpp \
    common/defendbutton.cpp \
    common/imgtipslabel.cpp \
    common/checkboximagelabel.cpp \
    common/checkupdatepushbutton.cpp \
    home/aboutdialog.cpp \
    common/imagebutton.cpp \
    common/licensebutton.cpp \
    common/himagebutton.cpp \
    common/checkboxlabel.cpp \
    home/scanbodywidget.cpp \
    home/scanbotbodywidget.cpp \
    home/scanmidbodywidget.cpp \
    home/scantopbodywidget.cpp \
    common/scanresulttable.cpp \
    home/stoppedtopbodywidget.cpp \
    home/virus/dirselectbotbodywidget.cpp \
    home/virus/dirselectdialog.cpp \
    home/virus/dirselectheaderwidget.cpp \
    home/virus/dirselectmidbodywidget.cpp \
    home/virus/dirselecttreeview.cpp \
    home/virus/dirselitem.cpp \
    home/virus/dirselpathproc.cpp \
    home/virus/dirseltreemodel.cpp \
    home/defenditemwidget.cpp \
    home/defendwidget.cpp \
    home/defendstatewidget.cpp \
    home/defendcenterdialog.cpp \
    home/defendcenterheaderwidget.cpp \
    home/defendcentertitlewidget.cpp \
    home/defendcentertopwidget.cpp \
    home/defendcentermidwidget.cpp \
    home/defendcenterbotwidget.cpp \
    home/defendcenteritems.cpp \
    home/blacklisturltable.cpp \
    home/blacklistiptable.cpp \
    home/virusmessagebox.cpp \
    home/scanprogresscircle.cpp \
    home/scanchart.cpp \
    pubfunc.cpp \
    ../../BaseController/application.cpp \
    ../../BaseController/appconfig.cpp \
    ../../BaseController/storagebridge.cpp \
    ../../CommonAPI/centerlistener.cpp \
    home/uicircularqueue.cpp \
    common/imgtxtlabel.cpp \
    common/modemenu.cpp \
    common/scanpausebtn.cpp \
    home/operationpower.cpp \
    common/updaterimagebutton.cpp \
    home/offlineupdateinfodialog.cpp


HEADERS  += home/mainwindow.h \
    home/titlewidget.h \
    home/bodywidget.h \
    home/virus/scanprogresswidget.h \
    home/virus/scanresultwidget.h \
    home/headerwidget.h \
    common/showbutton.h \
    common/pushbutton.h \
    common.h \
    common/dynmenu.h \
    common/animitedbutton.h \
    home/topbodywidget.h \
    home/midbodywidget.h \
    home/botbodywidget.h \
    common/defendbutton.h \
    common/imgtipslabel.h \
    common/checkboximagelabel.h \
    common/checkupdatepushbutton.h \
    common/checkboxlabel.h \
    home/aboutdialog.h \
    common/imagebutton.h \
    common/licensebutton.h \
    common/himagebutton.h \
    home/scanbodywidget.h \
    home/scanbotbodywidget.h \
    home/scanmidbodywidget.h \
    home/scantopbodywidget.h \
    common/scanresulttable.h \
    home/stoppedtopbodywidget.h \
    home/virus/dirselectbotbodywidget.h \
    home/virus/dirselectdialog.h \
    home/virus/dirselectheaderwidget.h \
    home/virus/dirselectmidbodywidget.h \
    home/virus/dirselecttreeview.h \
    home/virus/dirselitem.h \
    home/virus/dirselpathproc.h \
    home/virus/dirseltreemodel.h \
    home/defenditemwidget.h \
    home/defendwidget.h \
    home/defendstatewidget.h \
    home/defendcenterdialog.h \
    home/defendcenterheaderwidget.h \
    home/defendcentertitlewidget.h \
    home/defendcentertopwidget.h \
    home/defendcentermidwidget.h \
    home/defendcenterbotwidget.h \
    home/defendcenteritems.h \
    home/blacklisturltable.h \
    home/blacklistiptable.h \
    home/scanprogresscircle.h \
    home/scanchart.h \
    home/virusmessagebox.h \
    pubfunc.h \
    ../../BaseController/application.h \
    ../../BaseController/appconfig.h \
    ../../BaseController/storagebridge.h \
    ../../CommonAPI/centerlistener.h \
    ../../CommonAPI/commonapi.h \
    home/uicircularqueue.h \
    common/imgtxtlabel.h \
    common/modemenu.h \
    common/scanpausebtn.h \
    home/operationpower.h \
    common/updaterimagebutton.h \
    home/offlineupdateinfodialog.h

FORMS    +=

INCLUDEPATH += /opt/Qt5.9.2/5.9.2/gcc_64/include
INCLUDEPATH += common/
INCLUDEPATH += ./home/ += ../
INCLUDEPATH += ./
INCLUDEPATH += ../../BaseController/
INCLUDEPATH += ../../CommonAPI
INCLUDEPATH += ../../ScanEngine
INCLUDEPATH += ../../Storage
INCLUDEPATH += "../../Upgrade"
INCLUDEPATH += ../../ThirdPart/include



#LIBS += -L../../lib -lravstorage -lravcommon
LIBS += -L/home/rising/workspace/esm_dev/linux/build/3.0.0.1/lib
LIBS += -L/opt/BDFZ/RAV/lib -lravcrypto -lravstorage -lravcommon #-lravz
LIBS += -L/opt/Qt5.9.2/5.9.2/gcc_64/lib
#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))

#endif

RESOURCES += \
    ravresource.qrc

DISTFILES += \
    resource/common/Thumbs.db
