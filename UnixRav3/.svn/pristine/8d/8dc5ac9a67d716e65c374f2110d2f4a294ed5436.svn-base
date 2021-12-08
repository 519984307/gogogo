cmake_minimum_required (VERSION 2.8)
project(CommonAPI)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

link_directories(${PROJECT_SOURCE_DIR}/../ThirdPart/lib)
link_libraries(-lravcrypto)

set(LIBRARY_OUTPUT_PATH ${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/lib)

add_definitions(-DCOMMON_STRING_API -DCOMMON_HASH_API -DCOMMON_OS_API -DCOMMON_PATH_API -DCOMMON_JSON_API -DCOMMON_LOG_API -DCOMMON_7Z_API)
add_definitions(-D_7ZIP_ST)

#set(CMAKE_C_FLAGS   "-fpermissive")
#set(CMAKE_CXX_FLAGS   "-fpermissive")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O2")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc")

add_library(
    ravcommon
    SHARED
    commonapi.cpp
    handler.cpp
    communicatebundle.cpp
    configfile.cpp
    crypt/base64.cpp
    crypt/md5.cpp
    crypt/rc4.cpp
    crypt/crc32.c
    crypt/rscrypt.cpp
    json/json_internalarray.inl
    json/json_internalmap.inl
    json/json_reader.cpp
    json/json_value.cpp
    json/json_valueiterator.inl
    json/json_writer.cpp
    lzma/Alloc.c
    lzma/LzFind.c
    lzma/LzmaEnc.c
    lzma/LzmaDec.c
    lzma/7zFile.c
    lzma/7zStream.c
    lzma/7zCrc.c
    lzma/7zAlloc.c
    lzma/7zArcIn.c
    lzma/7zBuf.c
    lzma/7zBuf2.c
    lzma/7zBuf2.c
    lzma/7zCrcOpt.c
    lzma/7zDec.c
    lzma/CpuArch.c
    lzma/Delta.c
    lzma/Lzma2Dec.c
    lzma/Bra.c
    lzma/Bra86.c
    lzma/BraIA64.c
    lzma/Bcj2.c
    lzma/Ppmd7.c
    lzma/Ppmd7Dec.c
    lzma/rs7z.c
)

include_directories(
    ../ThirdPart/include
    ../BaseController
)

