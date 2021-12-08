cmake_minimum_required (VERSION 2.8)
project(RavConsoleApp)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(EXECUTABLE_OUTPUT_PATH ${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/bin)
set(CMAKE_AUTOMOC ON)

add_definitions(-DCOMMON_STRING_API -DCOMMON_HASH_API -DCOMMON_OS_API -DCOMMON_PATH_API -DCOMMON_JSON_API -DCOMMON_LOG_API -DCOMMON_ZIP_API)
add_definitions(-D${RPC_TYPE})

link_directories(${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/lib)
link_libraries(-lravcommon -lravstorage)

link_directories(${RavConsoleApp_SOURCE_DIR}/../../ThirdPart/lib)
link_libraries(-lravcrypto -lravncurses -lravz)
if (${COMM_TYPE} EQUAL 2)
    link_libraries(-lravev)
endif()

#add_definitions(-DCHINESE)

add_executable(
    ${RAV_CONSOLE_MAIN_APP}
    main.cpp
    view.cpp 
    textview.cpp 
    window.cpp 
    screen.cpp
    menu.cpp 
    titleview.cpp 
    dialog.cpp 
    listview.cpp 
    settingview.cpp 
    editview.cpp 
    optionview.cpp 
    aboutview.cpp
    ../../BaseController/application.cpp
    ../../BaseController/storagebridge.cpp
    ../../CommonAPI/centerlistener.cpp
    ../../BaseController/appconfig.cpp
)

include_directories(
    ../../CommonAPI
    ../../ScanEngine
    ../../BaseController
    ../../Communicator
    ../../Upgrade
    ../../Storage
    ../../unix_engine2013
    ../../ThirdPart/include/libxml2
    ../../ThirdPart/include
)

target_link_libraries(${RAV_CONSOLE_MAIN_APP} -lpthread)
