cmake_minimum_required (VERSION 2.8)
project(GLTX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(EXECUTABLE_OUTPUT_PATH ${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/bin)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

add_definitions(-DCOMMON_STRING_API -DCOMMON_HASH_API -DCOMMON_OS_API -DCOMMON_PATH_API -DCOMMON_JSON_API -DCOMMON_LOG_API -DCOMMON_7Z_API -DUNIXPLATFORM)
add_definitions(-D${RPC_TYPE})
add_definitions(-D_7ZIP_ST)

link_directories(${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/lib)
link_libraries(-lravcommon -lravstorage -lravcommunicator)


link_directories(${GLTX_SOURCE_DIR}/../../ThirdPart/lib)
link_libraries(-lravcrypto -lraviconv -lravxml2 -lravcurl -lravssl -lravz -g)
if (${COMM_TYPE} EQUAL 2)
    link_libraries(-lravev)
endif()

add_executable(
    ${RAV_DAEMON_GLTX}
    main.cpp
    mainengine.cpp
    clientinfo.cpp
    clientinfo_linux.cpp
    centerfindproxy.cpp
    scheduleengine.cpp
    ../COMMON/service.cpp
    ../COMMON/policysyncparser.cpp
    ../../CommonAPI/centerlistener.cpp
    ../../CommonAPI/zip/unixzip.cpp
    ../../BaseController/storagebridge.cpp
    ../../BaseController/appconfig.cpp
    ../../CommonAPI/zip/unixzip.cpp
    ../../Upgrade/Common/updateappengine.cpp
    ../../Upgrade/Common/updatevirusengine.cpp
    ../../Upgrade/Common/upgrade.cpp
    ../../Setup/cross.cpp
)

include_directories(
    ../COMMON
    ../../CommonAPI
    ../../BaseController
    ../../Storage
    ../../Communicator
    ../../ScanEngine
    ../../Upgrade
    ../../Setup
    ../../ThirdPart/include/libxml2
    ../../ThirdPart/include
    ../../Authorization
    ../../Authorization/rscommon
)

target_link_libraries(${RAV_DAEMON_GLTX} -lpthread)
target_link_libraries(${RAV_DAEMON_GLTX} -ldl)
target_link_libraries(${RAV_DAEMON_GLTX} ${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/lib/libravauth.a)


