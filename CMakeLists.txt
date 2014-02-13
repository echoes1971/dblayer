cmake_minimum_required(VERSION 2.8)
project(DBLayer)
# The version number.
set (Tutorial_VERSION_MAJOR 5)
set (Tutorial_VERSION_MINOR 2)

option (USE_LIBPQ      "Enable PostgreSQL support" OFF)
option (USE_LIBSQLITE3 "Enable Sqlite3 support"    OFF)
option (USE_MYSQL      "Enable MySQL support"      ON)
option (USE_ODBCPP     "Enable Odbc support"       OFF)
option (USE_LIBXMLRPC  "Enable XmlRPC support"     OFF)

configure_file (
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  )

message("PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")

include_directories ("${PROJECT_SOURCE_DIR}")

message("INCLUDE_DIRECTORIES: ${INCLUDE_DIRECTORIES}")

set (EXTRA_LIBS ${EXTRA_LIBS} Schema)
set (EXTRA_LIBS ${EXTRA_LIBS} DBLayer)

if (USE_LIBPQ)
    set (EXTRA_LIBS ${EXTRA_LIBS} pq)
endif (USE_LIBPQ)
if (USE_LIBSQLITE3)
    set (EXTRA_LIBS ${EXTRA_LIBS} sqlite3)
endif (USE_LIBSQLITE3)
if (USE_MYSQL)
    set (EXTRA_LIBS ${EXTRA_LIBS} mysqlclient)
endif (USE_MYSQL)
if (USE_ODBCPP)
    set (EXTRA_LIBS ${EXTRA_LIBS} odbc)
endif (USE_ODBCPP)
if (USE_LIBXMLRPC)
    set (EXTRA_LIBS ${EXTRA_LIBS} xmlrpc_client++ xmlrpc_client xmlrpc++ xmlrpc xmlrpc_util xmlrpc_xmlparse xmlrpc_xmltok)
endif (USE_LIBXMLRPC)

#include_directories ("${PROJECT_SOURCE_DIR}/dblayer")
add_subdirectory (schema)
add_subdirectory (dblayer)
add_subdirectory (src)

