cmake_minimum_required (VERSION 2.6)
project(RavConsoleUnsetup)

#set(CMAKE_EXE_LINKER_FLAGS "-static")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(EXECUTABLE_OUTPUT_PATH ${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/bin)

add_definitions(-DCOMMON_STRING_API -DCOMMON_HASH_API -DCOMMON_OS_API -DCOMMON_PATH_API -DCOMMON_ZIP_API -DWITHOUT_CRYPTO -DCOMMON_JSON_API)
add_definitions(-D${RPC_TYPE})
add_definitions(-DUNIXPLATFORM)

link_directories(${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/lib)
#link_libraries(-lravcommon)
link_directories(${RavConsoleUnsetup_SOURCE_DIR}/../../ThirdPart/lib)
#link_libraries(-lravcrypto -lravz)

include_directories(
    ../../Authorization
    ../../Authorization/rscommon
    ../../BaseController
    ../../CommonAPI
    ../../ScanEngine
    ../../Communicator
    ../../Storage
    ../../Setup
    ../../unix_engine2013/rame/lib3rd/json/src
    ../../ThirdPart/include
    ../../ThirdPart/include/libxml2
)

add_executable(
    ${RAV_CONSOLE_UNINSTALL}
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
    ravcmdunsetup.cpp
)

target_link_libraries(${RAV_CONSOLE_UNINSTALL} ${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/lib/libravauth.a)
target_link_libraries(${RAV_CONSOLE_UNINSTALL} ${PROJECT_SOURCE_DIR}/../../ThirdPart/lib/libravz.a)
target_link_libraries(${RAV_CONSOLE_UNINSTALL} ${PROJECT_SOURCE_DIR}/../../ThirdPart/lib/libravxml2.a)
#target_link_libraries(${RAV_CONSOLE_UNINSTALL} libstdc++.a)
link_directories(/usr/lib64)
target_link_libraries(${RAV_CONSOLE_UNINSTALL} libpthread.so)
target_link_libraries(${RAV_CONSOLE_UNINSTALL} libm.so)


