cmake_minimum_required (VERSION 2.6)
project(RJJH)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(EXECUTABLE_OUTPUT_PATH ${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/bin)

add_definitions(-DCOMMON_STRING_API  -DCOMMON_OS_API -DCOMMON_PATH_API -DWITHOUT_CRYPTO)
add_definitions(-D${RPC_TYPE})

link_directories(${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/lib)
link_libraries(-lravcommon -lravcommunicator)


link_directories(${Test_SOURCE_DIR}/../ThirdPart/lib)
link_libraries(-lravcrypto -lravcurl -lravssl -lravxml2 -lravz -lraviconv)

include_directories(
    ../../BaseController
    ../../CommonAPI
    ../../Storage
    ../../Setup
    ../../Communicator
    ../../ThirdPart/include
    ../../RavDaemon/GJCZ/
)

add_executable(
    RJJH
    main.cpp 
    ../../CommonAPI/centerlistener.cpp
    ../../CommonAPI/commonapi.cpp
    ../../CommonAPI/communicatebundle.cpp
    ../../CommonAPI/configfile.cpp
    ../../RavDaemon/GJCZ/clientinfo.cpp
    ../../RavDaemon/GJCZ/clientinfo_linux.cpp
)

target_link_libraries(RJJH -lpthread)
