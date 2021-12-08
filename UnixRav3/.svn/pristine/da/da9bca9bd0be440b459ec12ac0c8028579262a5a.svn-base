cmake_minimum_required (VERSION 2.8)
project(GJCZ)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(EXECUTABLE_OUTPUT_PATH ${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/bin)

if(${USB_CHECK_TYPE} STREQUAL "QT")
    set(Qt5Core_DIR ${QT5_LIBRARY_PATH}/lib/cmake/Qt5Core)
    find_package(Qt5Core REQUIRED)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

add_definitions(-DCOMMON_STRING_API -DCOMMON_HASH_API -DCOMMON_OS_API -DCOMMON_PATH_API -DCOMMON_JSON_API -DCOMMON_LOG_API -DCOMMON_ZIP_API)
add_definitions(-D${RPC_TYPE})
add_definitions(-DQT_NO_VERSION_TAGGING)
add_definitions(-DUPDATE_WITH_ENGINE_)

add_definitions("-Wall -g")


link_directories(${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/lib)
link_libraries(-lravcommon -lravstorage -lravscanengine)

link_directories(${GJCZ_SOURCE_DIR}/../../unix_engine2013/rame.bin)
link_libraries(-lrxbase)

link_directories(${GJCZ_SOURCE_DIR}/../../ThirdPart/lib)
link_libraries(-lravcrypto -lravcurl -lravssl -lravxml2 -lravz)
if (${COMM_TYPE} EQUAL 2)
    link_libraries(-lravev)
endif()

add_executable(
    ${RAV_DAEMON_GJCZ}
    main.cpp 
    mainengine.cpp 
    taskproxy.cpp 
    controller.cpp
    usbpluginchecker.cpp 
    scannotifycallback.cpp 
    pluginentry.cpp
    scanproxy.cpp
    simplejudger.cpp
    ../COMMON/service.cpp
    ../COMMON/policysyncparser.cpp
    ../../CommonAPI/centerlistener.cpp
    ../../BaseController/application.cpp
    ../../BaseController/storagebridge.cpp
    ../../BaseController/appconfig.cpp
    ../../Upgrade/Common/updatevirusengine.cpp
    ../../Upgrade/Common/upgrade.cpp
    ../../Communicator/models/policy/policymodel.cpp
    ../../Communicator/models/policy/commandmodel.cpp
    ../../Setup/cross.cpp
)

include_directories(
    ../COMMON
    ../../CommonAPI
    ../../ScanEngine
    ../../BaseController
    ../../Communicator
    ../../Upgrade
    ../../Storage
    ../../Setup
    ../../unix_engine2013/include
    ../../ThirdPart/include/libxml2
    ../../ThirdPart/include
)

target_link_libraries(${RAV_DAEMON_GJCZ} -lpthread)
target_link_libraries(${RAV_DAEMON_GJCZ} -ldl)

if(${USB_CHECK_TYPE} STREQUAL "QT")
    target_link_libraries(${RAV_DAEMON_GJCZ} Qt5::Core)
elseif(${USB_CHECK_TYPE} STREQUAL "UDEV")
    target_link_libraries(${RAV_DAEMON_GJCZ} -ludev)
endif()
