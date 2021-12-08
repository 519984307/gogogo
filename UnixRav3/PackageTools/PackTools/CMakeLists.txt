cmake_minimum_required (VERSION 2.6)
project(PackTool)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(EXECUTABLE_OUTPUT_PATH ${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/tools)

add_definitions(-DCOMMON_STRING_API -DCOMMON_HASH_API -DCOMMON_OS_API -DCOMMON_PATH_API -DCOMMON_ZIP_API)
add_definitions(-D${RPC_TYPE})

include_directories(
    ../../CommonAPI
    ../../CommonAPI/crypt
    ../../CommonAPI/zip
    ../../ThirdPart/include
)

add_executable(
    ${RAV_PACKAGE_TOOL}
    ../../CommonAPI/crypt/md5.cpp
    ../../CommonAPI/zip/unixzip.cpp
    ../../CommonAPI/zip/unzip.cpp
    ravpacksetup.cpp
 
)

target_link_libraries(${RAV_PACKAGE_TOOL} ${PROJECT_SOURCE_DIR}/../../ThirdPart/lib/libravz.a)
