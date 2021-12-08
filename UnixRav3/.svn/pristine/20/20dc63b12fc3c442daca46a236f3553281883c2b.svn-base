cmake_minimum_required (VERSION 3.1.0)
project(RavQtNotifier)

set(CMAKE_AUTOMOC ON)

if(${QT_VERSION} EQUAL 5)
    set(Qt5Widgets_DIR ${QT5_LIBRARY_PATH}/lib/cmake/Qt5Widgets)
    find_package(Qt5Widgets REQUIRED)
else()
    find_package(Qt4 4.8.6 REQUIRED QtCore QtGui)
endif()


add_definitions(-DCOMMON_STRING_API -DCOMMON_HASH_API -DCOMMON_OS_API -DCOMMON_PATH_API -DCOMMON_JSON_API -DCOMMON_LOG_API)
add_definitions(-D${RPC_TYPE})
add_definitions(-DQT_NO_VERSION_TAGGING)


set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(LIBRARY_OUTPUT_PATH ${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/lib)

link_directories(${RavQtNotifier_SOURCE_DIR}/../../ThirdPart/lib)
link_libraries(-lravcrypto)

link_directories(${BUILD_OUTPUT_DIRECTORY}/${BUILD_APP_VERSION}/lib)
link_libraries(-lravcommon)

if(${QT_VERSION} EQUAL 5)
    qt5_add_resources(
        qrc_FILES
        resource.qrc
    )
endif()

add_library(
    ravqtsupport
    SHARED
    main.cpp
    screenwidget.cpp
    popscreen.cpp
    pushbutton.cpp
    ../../CommonAPI/communicatebundle.cpp
    ${qrc_FILES}
)

include_directories(
    ../../BaseController
    ../../CommonAPI
    ../../ThirdPart/include
    ${QT5_LIBRARY_PATH}/include
    ${QT5_LIBRARY_PATH}/include/QtWidgets
    ${QT5_LIBRARY_PATH}/include/QtCore
    ${QT5_LIBRARY_PATH}/include/QtGui
)

target_link_libraries(ravqtsupport -lpthread)
if(${QT_VERSION} EQUAL 5)
    target_link_libraries(ravqtsupport Qt5::Widgets)
else()
    target_link_libraries(ravqtsupport Qt4::QtGui)
endif()
