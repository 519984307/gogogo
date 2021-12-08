cmake_minimum_required (VERSION 2.6)
project(GJCZ)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(EXECUTABLE_OUTPUT_PATH ${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/bin)

add_definitions(-DCOMMON_STRING_API  -DCOMMON_OS_API -DCOMMON_PATH_API -DWITHOUT_CRYPTO -DCOMMON_LOG_API -DCOMMON_HASH_API -DCOMMON_7Z_API -lCOMMON_ZIP_API)
add_definitions(-D${RPC_TYPE})

link_directories(${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/lib)
link_libraries(-lravcommon -lravauth)

link_directories(${GJCZ_SOURCE_DIR}/../../ThirdPart/lib)
link_libraries(-lravz -lravxml2 -lravcurl -lravssl -lravcrypto)
if (${COMM_TYPE} EQUAL 2)
    link_libraries(-lravev)
endif()

include_directories(
    ../../Authorization
    ../../BaseController
    ../../CommonAPI
    ../../Communicator
    ../../Storage
    ../../Setup
    ../../ThirdPart/include
    ../../ThirdPart/include/libxml2
    ../../Upgrade
    ../../ScanEngine
)

add_executable(
    GJCZ
    main.cpp 
    ../../CommonAPI/centerlistener.cpp
    ../../CommonAPI/commonapi.cpp
    ../../CommonAPI/communicatebundle.cpp
    ../../CommonAPI/configfile.cpp
    ../../CommonAPI/zip/unixzip.cpp
    ../../Upgrade/Common/upgrade.cpp
    ../../Upgrade/Common/updateappengine.cpp
    ../../Communicator/net/httprequest.cpp
)

target_link_libraries(GJCZ -lpthread)
#for mips
#target_link_libraries(GJCZ -lidn)
