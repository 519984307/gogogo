cmake_minimum_required (VERSION 2.6)
project(QuantineRestore)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(EXECUTABLE_OUTPUT_PATH ${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/tools)

add_definitions(-DCOMMON_STRING_API -DCOMMON_HASH_API -DCOMMON_OS_API -DCOMMON_PATH_API -DCOMMON_ZIP_API -DWITHOUT_CRYPTO -DCOMMON_JSON_API)
add_definitions(-D${RPC_TYPE})
add_definitions(-DUNIXPLATFORM)

link_directories(${PROJECT_SOURCE_DIR}/../../ThirdPart/lib)

link_directories(${GJCZ_SOURCE_DIR}/../../unix_engine2013/rame.bin)
link_libraries(-lrxbase)

include_directories(
    ../../ScanEngine
    ../../CommonAPI
    ../../CommonAPI/crypt
    ../../CommonAPI/zip
    ../../ThirdPart/include
    ../../ThirdPart/include/libxml2
    ../../unix_engine2013/include
)

add_executable(
    restore
    main.cpp
    ../../ScanEngine/ravquafile.cpp
    ../../ScanEngine/ravlockfile.cpp
)

