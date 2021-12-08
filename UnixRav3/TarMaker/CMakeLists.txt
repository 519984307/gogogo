cmake_minimum_required (VERSION 2.8)
project(TarMaker)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(EXECUTABLE_OUTPUT_PATH ${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/bin)

link_directories(${TarMaker_SOURCE_DIR}/../ThirdPart/lib)
link_libraries(-lpthread libravxml2.a)

aux_source_directory(. SOURCES) 
add_executable(${RAV_TARMAKER}
    ${SOURCES}
)

include_directories(
    ./
    ../ThirdPart/include
    ../ThirdPart/include/libxml2
    ../BaseController
)