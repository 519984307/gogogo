cmake_minimum_required (VERSION 2.8)
project(Upgrade)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(EXECUTABLE_OUTPUT_PATH ${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/bin)

add_definitions(-DCOMMON_STRING_API -DCOMMON_PATH_API -DCOMMON_OS_API -DCOMMON_JSON_API)
#set(CMAKE_C_FLAGS   "-fpermissive")
#set(CMAKE_CXX_FLAGS   "-fpermissive")

include_directories(
    ../../CommonAPI
    ../../ScanEngine
    ../../Storage
    ../../BaseController
    ../../Communicator
    ../../ThirdPart/include
    ../../ThirdPart/include/libxml2
    ../../unix_engine2013
    ../Common
)

add_executable(
    ${RAV_UPDATE}
    main.cpp
    ../Common/finishapp.cpp
    ../../Storage/cinifile.cpp
    ../../CommonAPI/configfile.cpp
    ../../CommonAPI/commonapi.cpp
    ../../BaseController/appconfig.cpp
    ../../CommonAPI/crypt/md5.cpp
    ../../CommonAPI/json/json_internalarray.inl
    ../../CommonAPI/json/json_internalmap.inl
    ../../CommonAPI/json/json_reader.cpp
    ../../CommonAPI/json/json_value.cpp
    ../../CommonAPI/json/json_valueiterator.inl
    ../../CommonAPI/json/json_writer.cpp
)

target_link_libraries(${RAV_UPDATE} -lpthread)
target_link_libraries(${RAV_UPDATE} ${PROJECT_SOURCE_DIR}/../../ThirdPart/lib/libravz.a)

