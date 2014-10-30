cmake_minimum_required(VERSION 2.8)


add_library(dblayer
    booleanfield.cpp
    dbconnection.cpp
    dbentity.cpp
    dblayer.cpp
    floatfield.cpp
    integerfield.cpp
    odbcconnection.cpp
    stringfield.cpp
    datefield.cpp
    dbefactory.cpp
    dbfield.cpp
    dbmgr.cpp
    foreignkey.cpp
    mysqlconnection.cpp
    sqliteconnection.cpp
    xmlrpcconnection.cpp

    #Default User Management
    authschema.cpp
    # Default Content Management
    cmschema.cpp
    )
