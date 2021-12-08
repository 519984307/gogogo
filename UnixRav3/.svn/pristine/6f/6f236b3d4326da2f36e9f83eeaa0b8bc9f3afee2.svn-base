cmake_minimum_required (VERSION 2.6)
project(Extracter)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(EXECUTABLE_OUTPUT_PATH ${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/bin)

add_definitions(-DCOMMON_STRING_API -DCOMMON_HASH_API -DCOMMON_OS_API -DCOMMON_PATH_API -DCOMMON_ZIP_API)
add_definitions(-D${RPC_TYPE})

include_directories(

)

add_executable(
    ${RAV_SELF_EXTRACTER}
    extract.cpp 
)
