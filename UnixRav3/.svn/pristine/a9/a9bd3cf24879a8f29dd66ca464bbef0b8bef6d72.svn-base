cmake_minimum_required (VERSION 2.6)
project(RAV_CROSS_UPDATER)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(EXECUTABLE_OUTPUT_PATH ${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/tools)

add_definitions(-DCOMMON_STRING_API -DCOMMON_HASH_API -DCOMMON_OS_API -DCOMMON_PATH_API -DCOMMON_ZIP_API -DWITHOUT_CRYPTO -DCOMMON_JSON_API)
add_definitions(-D${RPC_TYPE})
add_definitions(-DUNIXPLATFORM)

link_directories(${PROJECT_SOURCE_DIR}/../../ThirdPart/lib)

include_directories(
    ../../Authorization
    ../../Authorization/rscommon
    ../../BaseController
    ../../CommonAPI
    ../../ScanEngine
    ../../Storage
    ../../Setup
    ../../PackageTools/Downloader
    ../../RavConsole/Setup
    ../../ThirdPart/include
    ../../ThirdPart/include/openssl
    ../../ThirdPart/include/curl
    ../../ThirdPart/include/libxml2
)

add_executable(
    ${RAV_CROSS_UPDATER}
    ./main.cpp 
    ./updater.cpp
    ./tailreader.cpp
    ../../Setup/setup.cpp
    ../../Setup/cross.cpp
    ../../PackageTools/Downloader/dlparser.cpp
    ../../PackageTools/Downloader/httpdownloader.cpp
    ../../RavConsole/Setup/ravconsolesetup.cpp
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
)

target_link_libraries(${RAV_CROSS_UPDATER} ${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/lib/libravauth.a)
target_link_libraries(${RAV_CROSS_UPDATER} ${PROJECT_SOURCE_DIR}/../../ThirdPart/lib/libravz.a)
target_link_libraries(${RAV_CROSS_UPDATER} ${PROJECT_SOURCE_DIR}/../../ThirdPart/lib/libcurl.a)
target_link_libraries(${RAV_CROSS_UPDATER} ${PROJECT_SOURCE_DIR}/../../ThirdPart/lib/libssl.a)
target_link_libraries(${RAV_CROSS_UPDATER} ${PROJECT_SOURCE_DIR}/../../ThirdPart/lib/libcrypto.a)
target_link_libraries(${RAV_CROSS_UPDATER} ${PROJECT_SOURCE_DIR}/../../ThirdPart/lib/libravxml2.a)
#target_link_libraries(${RAV_CROSS_UPDATER} -lidn)
target_link_libraries(${RAV_CROSS_UPDATER} -lrt)
target_link_libraries(${RAV_CROSS_UPDATER} -ldl)
target_link_libraries(${RAV_CROSS_UPDATER} -lpthread)
