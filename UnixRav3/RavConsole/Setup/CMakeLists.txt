cmake_minimum_required (VERSION 2.6)
project(RavConsoleSetup)

#set(CMAKE_EXE_LINKER_FLAGS "-static")
#link_directories("/opt/rh/devtoolset-7/root/usr/lib/gcc/x86_64-redhat-linux/7/")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(EXECUTABLE_OUTPUT_PATH ${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/bin)

add_definitions(-DCOMMON_STRING_API -DCOMMON_HASH_API -DCOMMON_OS_API -DCOMMON_PATH_API -DCOMMON_ZIP_API -DWITHOUT_CRYPTO -DCOMMON_JSON_API)
add_definitions(-D${RPC_TYPE})
add_definitions(-DSETUP_SERIAL_CHECK)
add_definitions(-DUNIXPLATFORM)

link_directories(${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/lib)

#set(CMAKE_CXX_FLAGS   "-fpermissive")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O2")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc")


include_directories(
    ../../Authorization
    ../../Authorization/rscommon
    ../../BaseController
    ../../CommonAPI
    ../../ScanEngine
    ../../Storage
    ../../Setup
    ../../ThirdPart/include
    ../../ThirdPart/include/libxml2
)

add_executable(
    ${RAV_CONSOLE_SETUP}
    ../../Setup/setup.cpp
    ../../CommonAPI/configfile.cpp
    ../../CommonAPI/commonapi.cpp
    ../../CommonAPI/communicatebundle.cpp
    ../../CommonAPI/crypt/md5.cpp
    ../../CommonAPI/crypt/base64.cpp
    ../../CommonAPI/crypt/rc4.cpp
    ../../CommonAPI/crypt/rscrypt.cpp
    ../../CommonAPI/zip/unixzip.cpp
    ../../ScanEngine/base64.cpp
    ../../BaseController/appconfig.cpp
    ../../Storage/cinifile.cpp   
    ../../CommonAPI/json/json_reader.cpp
    ../../CommonAPI/json/json_writer.cpp
    ../../CommonAPI/json/json_value.cpp
    ravconsolesetup.cpp
    main.cpp
)

target_link_libraries(${RAV_CONSOLE_SETUP} ${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/lib/libravauth.a)
target_link_libraries(${RAV_CONSOLE_SETUP} ${PROJECT_SOURCE_DIR}/../../ThirdPart/lib/libravz.a)
target_link_libraries(${RAV_CONSOLE_SETUP} ${PROJECT_SOURCE_DIR}/../../ThirdPart/lib/libravxml2.a) 
target_link_libraries(${RAV_CONSOLE_SETUP} -ldl)
target_link_libraries(${RAV_CONSOLE_SETUP} -lpthread)
