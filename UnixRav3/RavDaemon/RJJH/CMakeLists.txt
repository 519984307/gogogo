cmake_minimum_required (VERSION 2.8)
project(RJJH)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SECRET  0)
#add_definitions(-DSECRET_MACHINE)

if (SECRET EQUAL 1)
    set(Qt5Widgets_DIR ${QT5_LIBRARY_PATH}/lib/cmake/Qt5Widgets)
    find_package(Qt5Widgets REQUIRED)
endif()

add_definitions(-DCOMMON_STRING_API -DCOMMON_HASH_API -DCOMMON_OS_API -DCOMMON_PATH_API -DCOMMON_JSON_API -DCOMMON_LOG_API)
add_definitions(-D${RPC_TYPE})

set(EXECUTABLE_OUTPUT_PATH ${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/bin)

link_directories(${RJJH_SOURCE_DIR}/../../ThirdPart/lib)
link_libraries(-lravcrypto)
if (${COMM_TYPE} EQUAL 2)
    link_libraries(-lravev)
endif()

link_directories(${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/lib)
link_libraries(-lravcommon -lravstorage)
if (SECRET EQUAL 1)
    link_libraries(-lravqtsupport)
endif()

include_directories(
    ../../CommonAPI
    ../../BaseController
    ../../Communicator
    ../../Storage
    ../../ScanEngine
    ../../Upgrade
    ../../Setup
    ../../ThirdPart/include
)

add_executable(
    ${RAV_NOTIFIY_APP}
    main.cpp 
    mainengine.cpp
    ../../CommonAPI/centerlistener.cpp
    ../../BaseController/storagebridge.cpp
    ../../BaseController/appconfig.cpp
    ../../Storage/cinifile.cpp
)

target_link_libraries(${RAV_NOTIFIY_APP} -lpthread)
target_link_libraries(${RAV_NOTIFIY_APP} -ldl)


if (SECRET EQUAL 1)
    target_link_libraries(${RAV_NOTIFIY_APP} Qt5::Widgets)
endif()
