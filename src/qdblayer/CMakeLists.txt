cmake_minimum_required(VERSION 2.8)


# ############### PATCH OSX: start. ##############
message("############### PATCH OSX: start. ##############")
set(QT5_PATH $ENV{HOME}/Qt/5.3/clang_64/ CACHE PATH "Path to Qt5")
set(QT5_MODULE_PATH ${QT5_PATH}/lib/cmake)
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${QT5_MODULE_PATH})
message("CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")
message("QT_QMAKE_EXECUTABLE: ${QT_QMAKE_EXECUTABLE}")

find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Widgets PATHS ~/Qt/)
find_package(Qt5Xml REQUIRED)

message("Qt5Core_INCLUDE_DIRS: ${Qt5Core_INCLUDE_DIRS}")
include_directories(${Qt5Core_INCLUDE_DIRS})

message("Qt5Network_INCLUDE_DIRS: ${Qt5Network_INCLUDE_DIRS}")
include_directories(${Qt5Network_INCLUDE_DIRS})

message("Qt5Widgets_INCLUDE_DIRS: ${Qt5Widgets_INCLUDE_DIRS}")
include_directories(${Qt5Widgets_INCLUDE_DIRS})
message("Qt5Widgets_EXECUTABLE_COMPILE_FLAGS: ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

message("Qt5Xml_INCLUDE_DIRS: ${Qt5Xml_INCLUDE_DIRS}")
include_directories(${Qt5Xml_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Core_EXECUTABLE_COMPILE_FLAGS} ${Qt5Network_EXECUTABLE_COMPILE_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS} ${Qt5Xml_EXECUTABLE_COMPILE_FLAGS}")
message("CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

set(CMAKE_AUTOMOC ON)
message("############### PATCH OSX: end. ##############")
# ############### PATCH OSX: end. ##############

include_directories("..")
include_directories(".")

add_library(qdblayer
    qdblayer.cpp
    qxmlrpcconnection.cpp
    qxmlrpcresultset.cpp

    qutexmlrpc/xrbase64.cpp
    qutexmlrpc/xrclient.cpp
    qutexmlrpc/xrmethodcall.cpp
    qutexmlrpc/xrmethodresponse.cpp
#    qutexmlrpc/xrserver.cpp
#    qutexmlrpc/xrsyncclient.cpp
    qutexmlrpc/xrvariant.cpp
    )

#target_link_libraries(qdblayer Qt::Core Qt::Network Qt::Widgets Qt::Xml)
qt5_use_modules(qdblayer Core Network Widgets Xml)
