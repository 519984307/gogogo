cmake_minimum_required (VERSION 2.8)
project(Upgrade)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(LIBRARY_OUTPUT_PATH ${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/lib)

add_definitions(-DCOMMON_PATH_API -DCOMMON_STRING_API -DCOMMON_LOG_API -DCOMMON_HASH_API -DCOMMON_OS_API)

link_directories(${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/lib)
link_libraries(-lravcommon -lravscanengine -lravcommunicator)

link_directories(${PROJECT_SOURCE_DIR}/../../unix_engine2013/rame.bin)
link_libraries(-lrxbase)

link_directories(${PROJECT_SOURCE_DIR}/../../ThirdPart/lib)
link_libraries(-lravcurl -lravssl -lravcrypto -lravxml2)

add_library(
    ravupgrade
    SHARED
    ../../Storage/cinifile.cpp 
    updateapp.cpp 
    upgrade.cpp
)

include_directories(
    ../../CommonAPI
    ../../ScanEngine
    ../../Storage
    ../../BaseController
    ../../Communicator
    ../../ThirdPart/include
    ../../ThirdPart/include/libxml2
    ../../unix_engine2013
)




