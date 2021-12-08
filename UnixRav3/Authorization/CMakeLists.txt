cmake_minimum_required (VERSION 2.8)
project(Authorization)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(LIBRARY_OUTPUT_PATH ${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/lib)

add_definitions(-DUNIXPLATFORM -DUNIX  -D_NOUSESHM_)

add_library(
    ravauth
    STATIC
    configimpl.cpp
    cfgapiimpl.cpp
    tree.cpp
    node.cpp
    checksn.cpp
    checkvalid.cpp
    cfgmutex.cpp
    shmem.cpp 
    ravbase64.cpp
    mutex.cpp 
    hash.cpp 
    unixrscfg.cpp 
    regapi.cpp
    md5/md5_c.cpp
    tinyxml/tinyxmlerror.cpp
    tinyxml/tinystr.cpp
    tinyxml/tinyxml.cpp
    tinyxml/tinyxmlparser.cpp
    ../ThirdPart/include/zlib/ioapi.c
    ../ThirdPart/include/zlib/unzip.c
    unzip/rsunzip.cpp
    rslicense/StdAfx.cpp
    rslicense/AesCbc.cpp
    rslicense/decrypt.cpp
    rslicense/P12.cpp
    rslicense/RisingAccredit.cpp
    rslicense/RisingSN.cpp
    rslicense/RsNetCRC16.cpp
    rslicense/RsNetRsa.cpp
    rslicense/rslicense.cpp
)

include_directories(
    ./rscommon
    ../ThirdPart/include
    ./md5
    ./tinyxml
)


