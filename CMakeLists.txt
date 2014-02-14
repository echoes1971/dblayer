cmake_minimum_required(VERSION 2.8)

project(DBLayer)

# The version number.
set (Tutorial_VERSION_MAJOR 5)
set (Tutorial_VERSION_MINOR 2)

############################################
# External libraries
############################################

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(MySQL)
find_package(ODBC)
find_package(PostgreSQL)
find_package(SQLite3)

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

message("Configuration")
message("=============")
message("")
message("MySQL: ${MYSQL_FOUND}")
message("Odbc: ${ODBC_FOUND}")
message("PostgreSQL: ${USE_LIBPQ}")
message("SQLite3: ${SQLITE3_FOUND}")
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

if (USE_LIBPQ)
    set (EXTRA_LIBS ${EXTRA_LIBS} pq)
endif (USE_LIBPQ)
if (USE_LIBXMLRPC)
    set (EXTRA_LIBS ${EXTRA_LIBS} xmlrpc_client++ xmlrpc_client xmlrpc++ xmlrpc xmlrpc_util xmlrpc_xmlparse xmlrpc_xmltok)
endif (USE_LIBXMLRPC)

if(MYSQL_FOUND)
 set (EXTRA_LIBS ${EXTRA_LIBS} mysqlclient)
endif(MYSQL_FOUND)
if (ODBC_FOUND)
    set (EXTRA_LIBS ${EXTRA_LIBS} odbc)
endif (ODBC_FOUND)
if (SQLITE3_FOUND)
    set (EXTRA_LIBS ${EXTRA_LIBS} sqlite3)
endif (SQLITE3_FOUND)


message("EXTRA_LIBS: ${EXTRA_LIBS}")


#include_directories ("${PROJECT_SOURCE_DIR}/dblayer")
add_subdirectory (schema)
add_subdirectory (dblayer)
add_subdirectory (src)

