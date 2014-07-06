cmake_minimum_required(VERSION 2.8.11)

project(QuteXmlrpc)

# cmake -DCMAKE_BUILD_TYPE=Debug . && make && ./testqute
# rm -rf cmake_install.cmake CMakeCache.txt CMakeFiles ; PATH=~/Qt/5.3/gcc_64/bin/:$PATH ; cmake . && make

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


include_directories("../")

add_library(qutexmlrpc
    xrbase64.cpp
    xrclient.cpp
    xrmethodcall.cpp
    xrmethodresponse.cpp
#    xrserver.cpp
#    xrsyncclient.cpp
    xrvariant.cpp
    )
#target_link_libraries(qutexmlrpc Qt::Core Qt::Network Qt::Widgets Qt::Xml)
qt5_use_modules(qutexmlrpc Core Network Widgets Xml)

# QT5_WRAP_UI(UIS_HDRS
#     client/client.ui
# )
# 
# message("UIS_HDRS: ${UIS_HDRS}")

add_executable(testqute
#    client/clientimpl.cpp
    client/test.cpp
#    ${UIS_HDRS}
    )
#qt5_use_modules(qutexmlrpc Core Network Widgets Xml)
target_link_libraries(testqute qutexmlrpc)
qt5_use_modules(testqute Core Network Widgets Xml)

