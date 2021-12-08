cmake_minimum_required (VERSION 2.8)
project(DefenModNotify)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

link_directories(${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/lib)
link_libraries(-lravcommon -lravstorage -ldl)

set(LIBRARY_OUTPUT_PATH ${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/lib)

add_definitions(-DCOMMON_STRING_API -DCOMMON_HASH_API -DCOMMON_OS_API -DCOMMON_PATH_API -DCOMMON_JSON_API -DCOMMON_LOG_API -DRAV_LOG)

add_library(
    ravdefenbinder_inotify
    SHARED
    main.cpp
    defenmodproxy.cpp
    defenplugins.cpp
    defenengine.cpp
    ../CommonAPI/crypt/crc32.c
    ../RavDaemon/COMMON/kernelapplication.cpp
)

add_library(
    ravdefenbinder_fanotify
    SHARED
    main.cpp
    defenmodproxy.cpp
    defenplugins.cpp
    defenengine.cpp
    ../CommonAPI/crypt/crc32.c
    ../RavDaemon/COMMON/kernelapplication.cpp
)
target_compile_options(ravdefenbinder_fanotify PRIVATE -DFANOTIFY_EXEC)

include_directories(
    ../ThirdPart/include
    ../CommonAPI
    ../Storage
    ../BaseController
    ../RavDaemon/COMMON
    ../ScanEngine
)


