cmake_minimum_required(VERSION 2.8)


#include_directories("..")
include_directories(".")

add_library(qdblayer
    qdblayer.cpp
    qxmlrpcconnection.cpp
    qresultset.cpp

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
