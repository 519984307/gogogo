cmake_minimum_required (VERSION 2.6)
project(AutoUpdate)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(EXECUTABLE_OUTPUT_PATH ${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/bin)

add_definitions(-DCOMMON_STRING_API -DCOMMON_OS_API -DCOMMON_PATH_API -DCOMMON_ZIP_API)
add_definitions(-D${RPC_TYPE})

include_directories(
    ../../CommonAPI
    ../../CommonAPI/crypt
    ../../CommonAPI/zip
    ../../Storage
    ../../BaseController
    ../../ThirdPart/include
)

add_executable(
    autoupdate.bin 
    autoupdate.cpp
    ../../CommonAPI/commonapi.cpp
    ../../CommonAPI/communicatebundle.cpp
    ../../CommonAPI/zip/unixzip.cpp
    ../../CommonAPI/crypt/md5.cpp
    ../../CommonAPI/json/json_reader.cpp
    ../../CommonAPI/json/json_writer.cpp
    ../../CommonAPI/json/json_value.cpp
    ../../BaseController/appconfig.cpp
)

target_link_libraries(autoupdate.bin ${PROJECT_SOURCE_DIR}/../../ThirdPart/lib/libravz.a)
target_link_libraries(autoupdate.bin -ldl)
