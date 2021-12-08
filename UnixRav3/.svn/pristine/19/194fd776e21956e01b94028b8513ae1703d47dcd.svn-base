cmake_minimum_required (VERSION 2.6)
project(Downloader)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(EXECUTABLE_OUTPUT_PATH ${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/bin)

add_definitions(-DCOMMON_STRING_API -DCOMMON_HASH_API -DCOMMON_OS_API -DCOMMON_PATH_API -DCOMMON_ZIP_API -DWITHOUT_CRYPTO -DCOMMON_JSON_API)
add_definitions(-D${RPC_TYPE})
add_definitions(-DUNIXPLATFORM)

link_directories(${PROJECT_SOURCE_DIR}/../../ThirdPart/lib)
link_libraries(-lravcrypto -lravcurl -lravssl)


include_directories(
    ../../ScanEngine
    ../../Authorization
    ../../Authorization/rscommon
    ../../Storage
    ../../BaseController
    ../../Setup
    ../../RavConsole/Setup
    ../../CommonAPI
    ../../CommonAPI/crypt
    ../../CommonAPI/zip
    ../../ThirdPart/include
    ../../ThirdPart/include/libxml2
)

add_executable(
    ${RAV_DOWNLOADER}
    ../../Setup/setup.cpp
    ../../RavConsole/Setup/ravconsolesetup.cpp
    ../../CommonAPI/crypt/md5.cpp
    ../../CommonAPI/crypt/base64.cpp
    ../../CommonAPI/crypt/rc4.cpp
    ../../CommonAPI/crypt/rscrypt.cpp
    ../../CommonAPI/zip/unixzip.cpp
    ../../CommonAPI/zip/unzip.cpp
    ../../CommonAPI/commonapi.cpp
    ../../CommonAPI/configfile.cpp
    ../../CommonAPI/communicatebundle.cpp
    ../../ScanEngine/base64.cpp
    ../../BaseController/appconfig.cpp
    ../../Storage/cinifile.cpp
    ../../CommonAPI/json/json_reader.cpp
    ../../CommonAPI/json/json_writer.cpp
    ../../CommonAPI/json/json_value.cpp
    httpdownloader.cpp
    dlparser.cpp
    downloader.cpp 
)

target_link_libraries(${RAV_DOWNLOADER} ${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/lib/libravauth.a)
target_link_libraries(${RAV_DOWNLOADER} ${PROJECT_SOURCE_DIR}/../../ThirdPart/lib/libravz.a)
target_link_libraries(${RAV_DOWNLOADER} ${PROJECT_SOURCE_DIR}/../../ThirdPart/lib/libravxml2.a)
