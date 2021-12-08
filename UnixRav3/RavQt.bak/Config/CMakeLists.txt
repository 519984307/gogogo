cmake_minimum_required (VERSION 3.1.0)
project(RavQtConfig)

if (QT_VERSION EQUAL 5)
    set(Qt5Widgets_DIR ${QT5_LIBRARY_PATH}/lib/cmake/Qt5Widgets)
    find_package(Qt5Widgets REQUIRED)
else()
    find_package(Qt4 4.8.6 REQUIRED QtCore QtGui)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(EXECUTABLE_OUTPUT_PATH ${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/bin)

add_definitions(-DCOMMON_STRING_API -DCOMMON_HASH_API -DCOMMON_OS_API -DCOMMON_PATH_API -DCOMMON_JSON_API -DCOMMON_LOG_API)
add_definitions(-D${RPC_TYPE})
add_definitions(-DQT_NO_VERSION_TAGGING)

link_directories(${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/lib)
link_libraries(-lravcommon -lravstorage )

link_directories(${RavQtConfig_SOURCE_DIR}/../../ThirdPart/lib)
link_libraries(-lravcrypto)
if (${COMM_TYPE} EQUAL 2)
    link_libraries(-lravev)
endif()

if (QT_VERSION EQUAL 5)
    qt5_add_resources(
        qrc_FILES
        resource.qrc
    )
#    qt5_wrap_ui(
#        qui_FILES
#        mainwindow.ui
#    )
endif()

include_directories(
    ../../CommonAPI
    ../../BaseController
    ../../Storage
    ../../ScanEngine
    ../../Upgrade
    ../../ThirdPart/include
    ./common
)

add_executable(
    ${RAV_QT_SETTING_APP}
    main.cpp 
    mainwindow.cpp 
    common/pushbutton.cpp 
    topwidget.cpp 
    leftwidget.cpp 
    common/imgtxtlabel.cpp 
    common/globalconf.cpp 
    origroup.cpp 
    common/checkboxlabel.cpp 
    whitelistgroup.cpp 
    common/filedirbutton.cpp 
    common/dirfiletable.cpp 
    common/extnametable.cpp 
    common/updatesourcetable.cpp 
    common/centerdiscoverytable.cpp 
    common/dirselectdialog.cpp 
    backupgroup.cpp 
    common/radiobutton.cpp 
    kvgroup.cpp 
    filegroup.cpp 
    udiskgroup.cpp 
    upgradegroup.cpp 
    centerdiscoverygroup.cpp 
    systemgroup.cpp 
    admingroup.cpp 
    traygroup.cpp 
    licensegroup.cpp 
    upgradegroup.cpp 
    appgroup.cpp 
    topscrollwidget.cpp 
    endscrollwidget.cpp 
    common/clickedlabel.cpp 
    common/imgbutton.cpp 
    righttopwidget.cpp 
    rightendwidget.cpp 
    common/combobox.cpp 
    timergroup.cpp 
    common/weekdayselector.cpp 
    netblacklistgroup.cpp 
    midscrollwidget.cpp 
    rightmidwidget.cpp
    operationpower.cpp 
    common/blacklisturltable.cpp 
    common/blacklistiptable.cpp 
    common/dirtable.cpp 
    common/procnametable.cpp
    common/privatecloudtable.cpp
    ../../CommonAPI/centerlistener.cpp
    ../../BaseController/application.cpp
    ../../BaseController/storagebridge.cpp
    ../../BaseController/appconfig.cpp
    ${qrc_FILES}
    ${qui_FILES}
)

if (QT_VERSION EQUAL 5)
    target_link_libraries(${RAV_QT_SETTING_APP} Qt5::Widgets)
else()
    target_link_libraries(${RAV_QT_SETTING_APP} Qt4::QtGui)
endif()
target_link_libraries(${RAV_QT_SETTING_APP} -lpthread)
