cmake_minimum_required (VERSION 2.8)
project(Communicator)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(LIBRARY_OUTPUT_PATH ${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/lib)

add_definitions(-DCOMMON_STRING_API -DCOMMON_HASH_API -DCOMMON_OS_API -DCOMMON_PATH_API -DCOMMON_JSON_API -DCOMMON_LOG_API)

link_directories(${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/lib)
link_libraries(-lravcommon)

link_directories(${PROJECT_SOURCE_DIR}/../ThirdPart/lib)
link_libraries(-lravcrypto -lravcurl -lravssl)

add_library(
    ravcommunicator
    SHARED
    communicator.cpp 
    models/log/logdatabasic.cpp 
    models/log/logdatalocation.cpp 
    models/log/logdatascanevent.cpp 
    models/log/logdataspammsg.cpp 
    models/log/logdataspamphone.cpp
    models/log/logdatavirus.cpp 
    models/log/logruamodel.cpp
    models/message/messagemodel.cpp 
    models/policy/commandmodel.cpp 
    models/policy/policymodel.cpp 
    models/contentfactory.cpp 
    net/apiservice.cpp 
    net/httprequest.cpp 
    net/headerfield.cpp
    models/client/clientinfomodel.cpp 
    models/log/logepstate.cpp 
    models/policy/authorizationmodel.cpp
    models/log/logcommandstate.cpp
)

include_directories(
    ./
    ../ThirdPart/include
    ../CommonAPI
    ../Storage
    ../BaseController
)



