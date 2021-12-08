cmake_minimum_required (VERSION 2.8.0)
project(EDRS)

set(CMAKE_C_FLAGS "-std=c99")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(EXECUTABLE_OUTPUT_PATH ${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/bin)

include_directories(${EDRS_SOURCE_DIR})

add_definitions(-D__WITH_MURMUR)

add_executable(
    ${RAV_DAEMON_EDRS}
    main.c
    clientinfo.c
    engine/coreengine.c
    engine/watchengine.c
    engine/scheduleengine.c
    hashtable/hashtable.c
    hashtable/murmur.c
    common/list_op.c
    common/hash_op.c
    common/math_op.c
    common/debug_op.c
    common/string_op.c
    common/inifile.c
    common/travel_op.c
    crypto/md5/md5.c
    common/md5_op.c
    entries/autostart.c
    entries/account.c
    entries/system.c
    entries/process.c
)

include_directories(
    /usr/include
    ./engine
)

target_link_libraries(${RAV_DAEMON_EDRS} -lpthread)
