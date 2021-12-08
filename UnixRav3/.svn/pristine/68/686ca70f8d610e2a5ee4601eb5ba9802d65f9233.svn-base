cmake_minimum_required (VERSION 2.8)
project(RJJH)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(EXECUTABLE_OUTPUT_PATH ${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/bin)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_definitions(-DCOMMON_STRING_API -DCOMMON_OS_API -DCOMMON_PATH_API)
add_definitions(-D${RPC_TYPE})

add_executable(
    ${RAV_CONTEXT_PROXY}
    main.cpp
    ../../CommonAPI/commonapi.cpp
    ../../CommonAPI/configfile.cpp
    ../../CommonAPI/communicatebundle.cpp
    ../../Storage/cinifile.cpp
)

include_directories(
    ../../CommonAPI
    ../../Storage
    ../../BaseController
)

target_link_libraries(${RAV_CONTEXT_PROXY} -lpthread)


