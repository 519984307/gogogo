cmake_minimum_required (VERSION 2.6)
project(RavWatcher)

set(EXECUTABLE_OUTPUT_PATH ${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/bin)

add_executable(
    ${RAV_WATCH_DOG}
    main.c
)

target_link_libraries(${RAV_WATCH_DOG} -lpthread)
