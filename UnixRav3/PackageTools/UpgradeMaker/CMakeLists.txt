cmake_minimum_required (VERSION 2.6)
project(UpgradeMaker)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(EXECUTABLE_OUTPUT_PATH ${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/tools)

add_definitions(-DCOMMON_STRING_API -DCOMMON_PATH_API -DCOMMON_7Z_API -DCOMMON_OS_API -DCOMMON_HASH_API)
add_definitions(-D_7ZIP_ST)
add_definitions(-D${RPC_TYPE})

include_directories(
    ../../CommonAPI
    ../../CommonAPI/crypt
    ../../BaseController
    ../../CommonAPI/lzma
    ../../ThirdPart/include/libxml2
)

link_directories(${UpgradeMaker_SOURCE_DIR}/../../ThirdPart/lib)
link_libraries(-lravcrypto -lravz -lraviconv -lravxml2)

add_executable(
    ${RAV_UPGRADE_MAKER}
    ../../CommonAPI/crypt/md5.cpp
    ../../CommonAPI/crypt/base64.cpp
    ../../CommonAPI/crypt/rc4.cpp
    ../../CommonAPI/lzma/7zAlloc.c
    ../../CommonAPI/lzma/7zBuf.c
    ../../CommonAPI/lzma/7zCrc.c
    ../../CommonAPI/lzma/7zCrcOpt.c
    ../../CommonAPI/lzma/7zFile.c
    ../../CommonAPI/lzma/7zDec.c
    ../../CommonAPI/lzma/7zArcIn.c
    ../../CommonAPI/lzma/7zStream.c
    ../../CommonAPI/lzma/Bcj2.c
    ../../CommonAPI/lzma/Bra.c
    ../../CommonAPI/lzma/Bra86.c
    ../../CommonAPI/lzma/BraIA64.c
    ../../CommonAPI/lzma/CpuArch.c
    ../../CommonAPI/lzma/Delta.c
    ../../CommonAPI/lzma/LzmaDec.c
    ../../CommonAPI/lzma/Lzma2Dec.c
    ../../CommonAPI/lzma/Ppmd7.c
    ../../CommonAPI/lzma/Ppmd7Dec.c
    ../../CommonAPI/lzma/LzmaEnc.c
    ../../CommonAPI/lzma/Lzma2Enc.c
    ../../CommonAPI/lzma/Alloc.c
    ../../CommonAPI/lzma/LzFind.c
    ../../CommonAPI/lzma/LzmaLib.c
    ../../CommonAPI/commonapi.cpp
    upgrademaker.cpp
    main.cpp
)

target_link_libraries(${RAV_UPGRADE_MAKER} -lz)
