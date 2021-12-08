cmake_minimum_required (VERSION 2.6)
project(CompileTool)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(EXECUTABLE_OUTPUT_PATH ${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/tools)

add_definitions(-DCOMMON_STRING_API -DCOMMON_HASH_API -DCOMMON_OS_API -DCOMMON_PATH_API -DCOMMON_JSON_API)
add_definitions(-D${RPC_TYPE})

set(CMAKE_EXE_LINKER_FLAGS "-static")

include_directories(
    ../../CommonAPI
    ../../BaseController
    ../../ThirdPart/include
)

add_executable(
    CompileTool
    main.cpp
    centerlistener.cpp
    mainengine.cpp
    ../../CommonAPI/commonapi.cpp
    ../../CommonAPI/communicatebundle.cpp
    ../../CommonAPI/crypt/md5.cpp
    ../../CommonAPI/crypt/base64.cpp
    ../../CommonAPI/crypt/rc4.cpp
    ../../CommonAPI/crypt/crc32.c
    ../../CommonAPI/crypt/rscrypt.cpp
    ../../CommonAPI/json/json_reader.cpp
    ../../CommonAPI/json/json_value.cpp
    ../../CommonAPI/json/json_writer.cpp
)

target_link_libraries(CompileTool -lpthread)
target_link_libraries(CompileTool ${PROJECT_SOURCE_DIR}/../../ThirdPart/lib/libcrypto.a)
