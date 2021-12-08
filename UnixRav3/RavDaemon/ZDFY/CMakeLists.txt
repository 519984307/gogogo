cmake_minimum_required (VERSION 2.8)
project(ZDFY)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(EXECUTABLE_OUTPUT_PATH ${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/bin)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

add_definitions(-DCOMMON_STRING_API -DCOMMON_HASH_API -DCOMMON_OS_API -DCOMMON_PATH_API -DCOMMON_JSON_API -DCOMMON_LOG_API)
add_definitions(-D${RPC_TYPE})

link_directories(${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/lib)
link_libraries(-lravcommon -lravstorage)

link_directories(${ZDFY_SOURCE_DIR}/../../ThirdPart/lib)
link_libraries(-lravcrypto -lraviconv -lravxml2 -lravz)
if (${COMM_TYPE} EQUAL 2)
    link_libraries(-lravev)
endif()

add_executable(
    ${RAV_DAEMON_ZDFY}
    main.cpp
    mainengine.cpp
    webserver.cpp
    ../COMMON/service.cpp
    ../COMMON/policysyncparser.cpp
    ../../CommonAPI/centerlistener.cpp
    ../../BaseController/application.cpp
    ../../BaseController/storagebridge.cpp
    ../../BaseController/appconfig.cpp
    ../../Communicator/models/policy/policymodel.cpp
    ../../Communicator/models/policy/commandmodel.cpp
)

include_directories(
    ../COMMON
    ../../CommonAPI
    ../../ScanEngine
    ../../BaseController
    ../../Communicator
    ../../Storage
    ../../Upgrade
    ../../ThirdPart/include/libxml2
    ../../ThirdPart/include
)

target_link_libraries(${RAV_DAEMON_ZDFY} -lpthread)
target_link_libraries(${RAV_DAEMON_ZDFY} -ldl)

