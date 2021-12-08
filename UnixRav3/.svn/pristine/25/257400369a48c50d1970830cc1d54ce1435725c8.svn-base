cmake_minimum_required (VERSION 3.1.0)
project(RavUninstall)

if (QT_VERSION EQUAL 5)
    set(Qt5Widgets_DIR ${QT5_LIBRARY_PATH}/lib/cmake/Qt5Widgets)
    find_package(Qt5Widgets REQUIRED)
else()
    find_package(Qt4 4.8.6 REQUIRED QtCore QtGui)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
add_definitions(-DCOMMON_STRING_API -DCOMMON_HASH_API -DCOMMON_OS_API -DCOMMON_PATH_API -DCOMMON_ZIP_API -DWITHOUT_CRYPTO -DCOMMON_JSON_API)

set(EXECUTABLE_OUTPUT_PATH ${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/bin)

add_definitions(-DQT_NO_VERSION_TAGGING)

link_directories(${QT5_LIBRARY_PATH}/lib)
#link_libraries(-licui18n)
link_directories(${RavConsoleUnsetup_SOURCE_DIR}/../../ThirdPart/lib)
#link_directories(${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/lib)


if (QT_VERSION EQUAL 5)
    qt5_wrap_ui(ui_FILES dialog.ui)
endif()

include_directories(
    ../../BaseController
    ../../CommonAPI
    ../../Communicator
    ../../Storage
    ../../Setup
    ../../unix_engine2013/rame/lib3rd/json/src
    ../../ThirdPart/include
    ../../ThirdPart/include/libxml2
)

add_executable(
    ${RAV_QT_UNINSTALL_APP}
    main.cpp 
    dialog.cpp 
    guiuninstaller.cpp
    ../../Setup/setup.cpp
    ../../CommonAPI/configfile.cpp
    ../../CommonAPI/commonapi.cpp
    ../../CommonAPI/communicatebundle.cpp
    ../../BaseController/appconfig.cpp
    ../../Storage/cinifile.cpp
    ../../CommonAPI/crypt/md5.cpp
    ../../CommonAPI/crypt/base64.cpp
    ../../CommonAPI/crypt/rc4.cpp
    ../../CommonAPI/zip/unixzip.cpp 
    ../../CommonAPI/json/json_reader.cpp
    ../../CommonAPI/json/json_writer.cpp
    ../../CommonAPI/json/json_value.cpp
    ${ui_FILES}
)

target_link_libraries(${RAV_QT_UNINSTALL_APP} ${PROJECT_SOURCE_DIR}/../../ThirdPart/lib/libravz.a)
target_link_libraries(${RAV_QT_UNINSTALL_APP} ${PROJECT_SOURCE_DIR}/../../ThirdPart/lib/libravxml2.a)
target_link_libraries(${RAV_QT_UNINSTALL_APP} libstdc++.a)

if (QT_VERSION EQUAL 5)
    target_link_libraries(${RAV_QT_UNINSTALL_APP} Qt5::Widgets)
else()
    target_link_libraries(${RAV_QT_UNINSTALL_APP} Qt4::QtGui)
endif()
