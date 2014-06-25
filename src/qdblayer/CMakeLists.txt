cmake_minimum_required(VERSION 2.8)


add_library(qdblayer
    qdblayer.cpp
    qxmlrpcconnection.cpp
    qxmlrpcresultset.cpp

    qutexmlrpc/xrbase64.cpp
    qutexmlrpc/xrclient.cpp
    qutexmlrpc/xrmethodcall.cpp
    qutexmlrpc/xrmethodresponse.cpp
    qutexmlrpc/xrserver.cpp
    qutexmlrpc/xrsyncclient.cpp
    qutexmlrpc/xrvariant.cpp
    )
# Use the Widgets module from Qt 5.

qt5_use_modules(qdblayer Core Network Widgets Xml)
