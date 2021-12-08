cmake_minimum_required (VERSION 3.1)
project(RJJHSEC)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

add_definitions(-DWITH_QT)

set(Qt5Widgets_DIR ${QT5_LIBRARY_PATH}/lib/cmake/Qt5Widgets)
find_package(Qt5Widgets REQUIRED)

add_definitions(-DCOMMON_STRING_API -DCOMMON_HASH_API -DCOMMON_OS_API -DCOMMON_PATH_API -DCOMMON_JSON_API -DCOMMON_LOG_API)
add_definitions(-D${RPC_TYPE})
add_definitions(-DQT_NO_VERSION_TAGGING)

set(EXECUTABLE_OUTPUT_PATH ${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/bin)

link_directories(${RJJHSEC_SOURCE_DIR}/../../ThirdPart/lib)
link_libraries(-lravcrypto)
if (${COMM_TYPE} EQUAL 2)
    link_libraries(-lravev)
endif()

link_directories(${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/lib)
link_libraries(-lravcommon -lravstorage)

include_directories(
    ../../CommonAPI
    ../../BaseController
    ../../Communicator
    ../../Storage
    ../../ScanEngine
    ../../Upgrade
    ../../Setup
    ../../RavDaemon/RJJH
    ../../RavQt/Notifier
    ../../ThirdPart/include

)

if(${QT_VERSION} EQUAL 5)
    qt5_add_resources(
        qrc_FILES
        ../../RavQt/Notifier/resource.qrc
    )
endif()

add_executable(
    ${RAV_NOTIFIY_SEC_APP}
    ../../RavQt/Notifier/main.cpp
    ../../RavQt/Notifier/screenwidget.cpp
    ../../RavQt/Notifier/popscreen.cpp
    ../../RavQt/Notifier/pushbutton.cpp
    ../RJJH/main.cpp 
    ../RJJH/mainengine.cpp
    ../../CommonAPI/centerlistener.cpp
    ../../BaseController/storagebridge.cpp
    ../../BaseController/appconfig.cpp
    ../../Storage/cinifile.cpp
    ../../CommonAPI/communicatebundle.cpp
    ${qrc_FILES}
)

target_link_libraries(${RAV_NOTIFIY_SEC_APP} -lpthread)
target_link_libraries(${RAV_NOTIFIY_SEC_APP} -ldl)
target_link_libraries(${RAV_NOTIFIY_SEC_APP} Qt5::Widgets)

