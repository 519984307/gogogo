cmake_minimum_required (VERSION 2.8)
project(ScanEngine)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(LIBRARY_OUTPUT_PATH ${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/lib)

add_definitions(-DCOMMON_STRING_API -DCOMMON_PATH_API -DCOMMON_LOG_API -DCOMMON_OS_API)

link_directories(${ScanEngine_SOURCE_DIR}/../ThirdPart/lib)
link_libraries(-lravcrypto -lravxml2 -lravcurl)

#set(CMAKE_C_FLAGS   "-fpermissive")
#set(CMAKE_CXX_FLAGS   "-fpermissive")

#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc")

add_definitions("-Wall -g")

add_library(
    ravscanengine
    SHARED
    scanengine.cpp 
    cloudengine.cpp
    multivirusengine.cpp    
    utils.cpp 
    ravquafile.cpp 
    ravlockfile.cpp
    scanthreadvari.cpp 
    countfilevari.cpp 
    get_cpu.c
    scancache.cpp
    base64.cpp
    rsupgradecode.cpp
    ../CommonAPI/commonapi.cpp

)

include_directories(
    ../unix_engine2013/include
    ../Storage
    ../CommonAPI
    ../BaseController
    ../ThirdPart/include
    ../ThirdPart/include/libxml2
)



