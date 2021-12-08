cmake_minimum_required (VERSION 3.1.0)
project(RavQtLogView)

if (QT_VERSION EQUAL 5)
    set(Qt5Widgets_DIR ${QT5_LIBRARY_PATH}/lib/cmake/Qt5Widgets)
    find_package(Qt5Widgets REQUIRED)
else()
    find_package(Qt4 4.8.6 REQUIRED QtCore QtGui)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(EXECUTABLE_OUTPUT_PATH ${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/bin)

add_definitions(-DCOMMON_STRING_API -DCOMMON_HASH_API -DCOMMON_OS_API -DCOMMON_PATH_API -DCOMMON_JSON_API -DCOMMON_LOG_API -DCOMMON_ZIP_API)
add_definitions(-D${RPC_TYPE})
add_definitions(-DQT_NO_VERSION_TAGGING)


link_directories(${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/lib)
link_libraries(-lravcommon -lravstorage )

link_directories(${RavQtLogView_SOURCE_DIR}/../../ThirdPart/lib)
link_libraries(-lravcrypto)
if (${COMM_TYPE} EQUAL 2)
    link_libraries(-lravev)
endif()

if (QT_VERSION EQUAL 5)
    qt5_add_resources(
        qrc_FILES
        resource.qrc
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
)

add_executable(
    ${RAV_QT_LOGVIEW_APP}
    main.cpp 
    mainwindow.cpp 
    leftwidget.cpp 
    topwidget.cpp 
    common/clickedlabel.cpp 
    common/combobox.cpp 
    common/dirfiletable.cpp 
    common/dirselectdialog.cpp 
    common/extnametable.cpp 
    common/filedirbutton.cpp 
    common/globalconf.cpp 
    common/imgbutton.cpp 
    common/imagebutton.cpp 
    common/imgtxtlabel.cpp 
    common/pushbutton.cpp 
    common/radiobutton.cpp 
    common/sysmaxbtn.cpp 
    confirmrestore.cpp 
    virusdetailgroup.cpp 
    scaneventgroup.cpp 
    sysreinforcegroup.cpp 
    appreinforcegroup.cpp 
    isolationgroup.cpp 
    urlaccessgroup.cpp 
    iprulegroup.cpp 
    common/checkboxlabel.cpp 
    rightwidget.cpp 
    common/searchedit.cpp 
    sysloggroup.cpp 
    installgroup.cpp 
    operationpower.cpp 
    ../../BaseController/application.cpp
    ../../BaseController/storagebridge.cpp
    ../../BaseController/appconfig.cpp
    ../../CommonAPI/centerlistener.cpp
    ${qrc_FILES}
)

if (QT_VERSION EQUAL 5)
    target_link_libraries(${RAV_QT_LOGVIEW_APP} Qt5::Widgets)
else()
    target_link_libraries(${RAV_QT_LOGVIEW_APP} Qt4::QtGui)
endif()
target_link_libraries(${RAV_QT_LOGVIEW_APP} -lpthread)

