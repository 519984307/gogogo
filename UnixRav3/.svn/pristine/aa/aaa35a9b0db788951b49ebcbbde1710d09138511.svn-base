cmake_minimum_required (VERSION 2.6)
project(AutoRepairMaker)

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
   autorepair.bin
   autorepair.cpp 
   ../../CommonAPI/commonapi.cpp
   ../../CommonAPI/zip/unixzip.cpp
   ../../CommonAPI/crypt/md5.cpp
)


target_link_libraries(autorepair.bin ${PROJECT_SOURCE_DIR}/../../ThirdPart/lib/libravz.a)

