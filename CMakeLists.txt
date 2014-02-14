cmake_minimum_required(VERSION 2.8)

project(DBLayer)

# https://bitbucket.org/catanasoaei/nanocv/src/fd9713422b20f1129b211c88d77151d8f7b38377/CMakeLists.txt?at=dev

# The version number.
set (Tutorial_VERSION_MAJOR 5)
set (Tutorial_VERSION_MINOR 2)

# Changing year of copyright:
# sed -i "s/2012 by /2014 by /g" *.h
# sed -i "s/2012 by /2014 by /g" *.cpp

############################################
# External libraries
############################################

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(MySQL)
find_package(ODBC)
find_package(PostgreSQL)
find_package(SQLite3)
find_package(XmlrpcC)

if(MYSQL_FOUND)
 set(USE_MYSQL TRUE)
endif(MYSQL_FOUND)
if(ODBC_FOUND)
 set(USE_ODBCPP TRUE)
endif(ODBC_FOUND)
if(POSTGRESQL_INCLUDE_DIR)
 set(USE_LIBPQ TRUE)
endif(POSTGRESQL_INCLUDE_DIR)
if(SQLITE3_FOUND)
 set(USE_LIBSQLITE3 TRUE)
endif(SQLITE3_FOUND)
if(XMLRPCC_FOUND)
 set(USE_LIBXMLRPC TRUE)
endif(XMLRPCC_FOUND)

message("Configuration")
message("=============")
message("")
message("MySQL: ${MYSQL_FOUND}")
message("Odbc: ${ODBC_FOUND}")
message("PostgreSQL: ${USE_LIBPQ}")
message("SQLite3: ${SQLITE3_FOUND}")
message("Xmlrpc-C: ${XMLRPCC_FOUND}")
message("")

configure_file(
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  )

############################################
# Build
############################################

message("PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")

include_directories ("${PROJECT_SOURCE_DIR}")

message("INCLUDE_DIRECTORIES: ${INCLUDE_DIRECTORIES}")

set (EXTRA_LIBS ${EXTRA_LIBS} Schema)
set (EXTRA_LIBS ${EXTRA_LIBS} DBLayer)

if(MYSQL_FOUND)
 set (EXTRA_LIBS ${EXTRA_LIBS} mysqlclient)
endif(MYSQL_FOUND)
if (ODBC_FOUND)
 set (EXTRA_LIBS ${EXTRA_LIBS} odbc)
endif (ODBC_FOUND)
if (USE_LIBPQ)
 set (EXTRA_LIBS ${EXTRA_LIBS} pq)
endif (USE_LIBPQ)
if (SQLITE3_FOUND)
 set (EXTRA_LIBS ${EXTRA_LIBS} sqlite3)
endif (SQLITE3_FOUND)
if (USE_LIBXMLRPC)
 set (EXTRA_LIBS ${EXTRA_LIBS} xmlrpc_client++ xmlrpc_client xmlrpc++ xmlrpc xmlrpc_util xmlrpc_xmlparse xmlrpc_xmltok)
endif (USE_LIBXMLRPC)


message("EXTRA_LIBS: ${EXTRA_LIBS}")


#include_directories ("${PROJECT_SOURCE_DIR}/dblayer")
add_subdirectory (schema)
add_subdirectory (dblayer)
add_subdirectory (src)

