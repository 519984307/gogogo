cmake_minimum_required (VERSION 2.8)
project(Caja)

# important
# TO compile the caja extention, you must install
# libcaja-extension-dev
# glib2.0
# gtk-3.0
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(LIBRARY_OUTPUT_PATH ${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/plugins)

add_definitions(-DCOMMON_STRING_API -DCOMMON_HASH_API -DCOMMON_OS_API -DCOMMON_PATH_API -DCOMMON_JSON_API -DCOMMON_LOG_API -DCOMMON_ZIP_API)

add_library(
    caja-rav
    SHARED
    caja-rav.c
    rav-module.c
)

include_directories(
    ../../BaseController
    ../ThirdPart/include
    /usr/include/caja
    /usr/include/glib-2.0
    /usr/include/gtk-3.0
    /usr/lib/x86_64-linux-gnu/glib-2.0/include
    /usr/include/pango-1.0
    /usr/include/cairo
    /usr/include/gdk-pixbuf-2.0
    /usr/include/atk-1.0
)


