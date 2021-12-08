cmake_minimum_required (VERSION 2.6)
project(SETUP)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(EXECUTABLE_OUTPUT_PATH ${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/bin)

add_definitions(-DCOMMON_STRING_API  -DCOMMON_OS_API -DCOMMON_PATH_API -DWITHOUT_CRYPTO)
add_definitions(-D${RPC_TYPE})

include_directories(
    ../../BaseController
    ../../CommonAPI
    ../../Storage
    ../../Setup
    ../../ThirdPart/include
    ../../ThirdPart/include/libxml2
)

add_executable(
    SETUP
    main.cpp 
    ../../CommonAPI/commonapi.cpp
    ../../Setup/cross.cpp
    ../../Storage/cinifile.cpp
    ../../CommonAPI/crypt/base64.cpp
)

target_link_libraries(SETUP -ldl)
target_link_libraries(SETUP ${PROJECT_SOURCE_DIR}/../../ThirdPart/lib/libravxml2.a)
