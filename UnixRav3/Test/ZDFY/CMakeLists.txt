cmake_minimum_required (VERSION 2.6)
project(ZDFY)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(EXECUTABLE_OUTPUT_PATH ${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/bin)

add_definitions(-DCOMMON_STRING_API -DCOMMON_OS_API -DCOMMON_PATH_API  -DWITHOUT_CRYPTO)
add_definitions(-D${RPC_TYPE})

include_directories(
    ../../BaseController
    ../../CommonAPI
    ../../Storage
    ../../RavDaemon
    ../../ThirdPart/include
)

add_executable(
    ZDFY
    main.cpp 
    ../../RavDaemon/defenmodproxy.cpp
    ../../RavDaemon/ukcom_u.c
    ../../CommonAPI/commonapi.cpp
    ../../CommonAPI/configfile.cpp
)

target_link_libraries(ZDFY -lpthread)
