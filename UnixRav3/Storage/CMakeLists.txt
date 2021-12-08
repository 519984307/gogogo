cmake_minimum_required (VERSION 2.6)
project(Storage)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(LIBRARY_OUTPUT_PATH ${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/lib)

add_definitions(-DCOMMON_STRING_API)
add_definitions(-DSQLITE_DISABLE_LFS)


add_library(
    ravstorage
    SHARED
    storage.cpp 
    cinifile.cpp 
    Entry/settingentry.cpp 
    SQLite3/sqlite3.c 
    Entry/baseentry.cpp 
    CPPSQLite/cppsqlite3.cpp 
    Entry/virusinfoentry.cpp 
    Entry/logevententry.cpp 
    Entry/scheduleentry.cpp 
    Entry/whitelistentry.cpp 
    Entry/commeventlog.cpp 
    Entry/quarantinelog.cpp 
    Entry/netaccessentry.cpp 
    Entry/urlruleentry.cpp 
    Entry/sourcelistentry.cpp
    ../CommonAPI/commonapi.cpp
)

include_directories(
    ../CommonAPI
    ../BaseController
    ./SQLite3
)



