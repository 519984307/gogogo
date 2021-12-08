cmake_minimum_required (VERSION 2.8.0)
project(Netfilter)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

link_directories(${PROJECT_SOURCE_DIR}../ThirdPart/lib)


set(LIBRARY_OUTPUT_PATH ${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/lib)

add_definitions(-DCOMMON_STRING_API -DCOMMON_HASH_API -DCOMMON_OS_API -DCOMMON_PATH_API -DCOMMON_JSON_API -DCOMMON_LOG_API)

add_library(
    ravfilterbinder
    SHARED
    main.cpp
    filterengine.cpp
    NFLCommunicator.cpp
    RUserNFLink.cpp
    ../RavDaemon/COMMON/kernelapplication.cpp
)

include_directories(
    ../ThirdPart/include
    ../CommonAPI
    ../Storage
    ../BaseController
    ../RavDaemon/COMMON
)


