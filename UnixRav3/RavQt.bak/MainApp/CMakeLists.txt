cmake_minimum_required (VERSION 3.1.0)
project(RavQtMainApp)

if (QT_VERSION EQUAL 5)
    set(Qt5Widgets_DIR ${QT5_LIBRARY_PATH}/lib/cmake/Qt5Widgets)
    find_package(Qt5Widgets REQUIRED)
else()
    find_package(Qt4 4.8.7 REQUIRED QtCore QtGui)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(EXECUTABLE_OUTPUT_PATH ${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/bin)

add_definitions(-DCOMMON_STRING_API -DCOMMON_OS_API -DCOMMON_PATH_API -DCOMMON_JSON_API -DCOMMON_LOG_API)
add_definitions(-D${RPC_TYPE})
add_definitions(-DQT_NO_VERSION_TAGGING)
add_definitions(-DQT_NO_STL)

link_directories(${RavQtMainApp_SOURCE_DIR}/../../ThirdPart/lib)
link_libraries(-lravcrypto)
if (${COMM_TYPE} EQUAL 2)
    link_libraries(-lravev)
endif()

link_directories(${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/lib)
link_libraries(-lravcommon -lravstorage)

if (QT_VERSION EQUAL 5)
    qt5_add_resources(
        qrc_FILES
        ravresource.qrc
    )
endif()

include_directories(
    ../../CommonAPI
    ../../BaseController
    ../../Storage
    ../../ScanEngine
    ../../Upgrade
    ../../ThirdPart/include
    ./common
    ./home
    ./backsys
    )

add_executable(
    ${RAV_QT_MAIN_APP}
    main.cpp 
    home/mainwindow.cpp 
    home/titlewidget.cpp 
    home/bodywidget.cpp 
    home/virus/scanprogresswidget.cpp 
    home/virus/scanresultwidget.cpp 
    home/headerwidget.cpp 
    common/showbutton.cpp 
    common/pushbutton.cpp 
    common/dynmenu.cpp 
    common/animitedbutton.cpp 
    home/topbodywidget.cpp 
    home/midbodywidget.cpp 
    home/botbodywidget.cpp 
    common/defendbutton.cpp 
    common/imgtipslabel.cpp 
    common/checkboximagelabel.cpp 
    common/checkboxlabel.cpp
    common/checkupdatepushbutton.cpp 
    common/imgtxtlabel.cpp
    common/globalconf.cpp
    common/modemenu.cpp
    home/aboutdialog.cpp 
    common/imagebutton.cpp 
    common/licensebutton.cpp 
    common/himagebutton.cpp 
    common/scanpausebtn.cpp
    home/scanbodywidget.cpp 
    home/scanbotbodywidget.cpp 
    home/scanmidbodywidget.cpp 
    home/scantopbodywidget.cpp 
    common/scanresulttable.cpp 
    home/stoppedtopbodywidget.cpp 
    home/virus/dirselectbotbodywidget.cpp 
    home/virus/dirselectdialog.cpp 
    home/virus/dirselectheaderwidget.cpp 
    home/virus/dirselectmidbodywidget.cpp 
    home/virus/dirselecttreeview.cpp 
    home/virus/dirselitem.cpp 
    home/virus/dirselpathproc.cpp 
    home/virus/dirseltreemodel.cpp 
    home/defenditemwidget.cpp 
    home/defendwidget.cpp 
    home/defendstatewidget.cpp 
    home/defendcenterdialog.cpp 
    home/defendcenterheaderwidget.cpp 
    home/defendcentertitlewidget.cpp 
    home/defendcentertopwidget.cpp 
    home/defendcentermidwidget.cpp 
    home/defendcenterbotwidget.cpp 
    home/defendcenteritems.cpp 
    home/blacklisturltable.cpp 
    home/blacklistiptable.cpp 
    home/scanchart.cpp
    home/scanprogresscircle.cpp
    home/uicircularqueue.cpp
    home/operationpower.cpp
    home/offlineupdateinfodialog.cpp
    pubfunc.cpp 
    home/virusmessagebox.cpp
    ../../CommonAPI/centerlistener.cpp
    ../../BaseController/application.cpp
    ../../BaseController/storagebridge.cpp
    ../../BaseController/appconfig.cpp
    ${qrc_FILES}
)

if (QT_VERSION EQUAL 5)
    target_link_libraries(${RAV_QT_MAIN_APP} Qt5::Widgets)
else()
    target_link_libraries(${RAV_QT_MAIN_APP} Qt4::QtGui)
endif()
target_link_libraries(${RAV_QT_MAIN_APP} -lpthread)
target_link_libraries(${RAV_QT_MAIN_APP} -lpthread)

