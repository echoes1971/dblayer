cmake_minimum_required(VERSION 2.8.11)

project(DBLayer)

# https://bitbucket.org/catanasoaei/nanocv/src/fd9713422b20f1129b211c88d77151d8f7b38377/CMakeLists.txt?at=dev

ADD_DEFINITIONS(
    -std=c++11 # Or -std=c++0x
    # Other flags
)

# The version number.
set(DBLayer_VERSION_MAJOR 5)
set(DBLayer_VERSION_MINOR 3)

# Changing year of copyright:
# sed -i "s/2012 by /2014 by /g" *.h
# sed -i "s/2012 by /2014 by /g" *.cpp

# Configuration file
# configure_file(
#         "${CMAKE_SOURCE_DIR}/config.h.in"
#         "${CMAKE_BINARY_DIR}/config.h")

############################################
# External libraries
############################################

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../cmake/Modules/")

find_package(MySQL)
find_package(ODBC)
find_package(PostgreSQL)
find_package(SQLite3)
find_package(XmlrpcC)

find_package(Boost)
find_package(LibMagic)
find_package(OpenSSL)

find_package(Qt5Widgets PATHS ~/Qt/)

if(MYSQL_FOUND)
 set(USE_MYSQL TRUE)
endif()
if(ODBC_FOUND)
 set(USE_ODBCPP TRUE)
endif()
if(POSTGRESQL_INCLUDE_DIR)
 set(USE_LIBPQ TRUE)
endif()
if(SQLITE3_FOUND)
 set(USE_LIBSQLITE3 TRUE)
endif()
if(XMLRPCC_FOUND)
 set(USE_LIBXMLRPC TRUE)
endif()
if(Qt5Widgets_FOUND)
 set(USE_QXMLRPC TRUE)
 message("PATH: ${PATH}")

 # NOTE: this MUST be before the SQLite include, because it will add
 # the path /opt/local/include with the old and wrong qt5 includes

 find_package(Qt5Core REQUIRED)
 message("Qt5Core_INCLUDE_DIRS: ${Qt5Core_INCLUDE_DIRS}")
 include_directories(${Qt5Core_INCLUDE_DIRS})

 find_package(Qt5Network REQUIRED)
 include_directories(${Qt5Network_INCLUDE_DIRS})

 include_directories(${Qt5Widgets_INCLUDE_DIRS})

 find_package(Qt5Gui REQUIRED)
 include_directories(${Qt5Gui_INCLUDE_DIRS})

 find_package(Qt5Xml REQUIRED)
 include_directories(${Qt5Xml_INCLUDE_DIRS})

 #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}
 #    ${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")
 set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS} -fPIC")

 #set(EXTRA_LIBS ${EXTRA_LIBS} Qt5::Core Qt5::Network Qt5::Widgets Qt5::Xml)

 # Find includes in corresponding build directories
 #set(CMAKE_INCLUDE_CURRENT_DIR ON)

 # Instruct CMake to run moc automatically when needed.
 set(CMAKE_AUTOMOC ON)

endif()
if(Boost_FOUND)
 set(USE_BOOST TRUE)
endif()
if(MAGIC_FOUND)
 set(USE_LIBMAGIC TRUE)
endif()
if(OPENSSL_VERSION)
 set(USE_OPENSSL TRUE)
endif()

message("Configuration")
message("=============")
message("")
message("MySQL: ${MYSQL_FOUND}")
message("Odbc: ${ODBC_FOUND}")
message("PostgreSQL: ${USE_LIBPQ}")
message("SQLite3: ${SQLITE3_FOUND}")
message("Xmlrpc-C: ${XMLRPCC_FOUND}")
message("")
message("Qt5: ${Qt5Widgets_FOUND}")
message("")
message("Boost: ${Boost_FOUND}")
message("LibMagic: ${MAGIC_FOUND}")
message("OpenSSL: ${OPENSSL_VERSION}")
message("")

configure_file(
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  )

############################################
# Build
############################################

message("PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")

include_directories(
        ${PROJECT_BINARY_DIR}             # config.h
        ${PROJECT_SOURCE_DIR})


set(EXTRA_LIBS ${EXTRA_LIBS} schema)
set(EXTRA_LIBS ${EXTRA_LIBS} dblayer)

if(MYSQL_FOUND)
 include_directories(${MYSQL_INCLUDE_DIR})
 set(EXTRA_LIBS ${EXTRA_LIBS} mysqlclient)
endif()
if(ODBC_FOUND)
 set(EXTRA_LIBS ${EXTRA_LIBS} odbc)
endif()
if(USE_LIBPQ)
 set(EXTRA_LIBS ${EXTRA_LIBS} pq)
 include_directories(${POSTGRESQL_INCLUDE_DIR})
endif()
if(SQLITE3_FOUND)
 set(EXTRA_LIBS ${EXTRA_LIBS} ${SQLITE3_LIBRARY})
 include_directories(${SQLITE3_INCLUDE_DIR})
endif()
if(USE_LIBXMLRPC)
 set(EXTRA_LIBS ${EXTRA_LIBS} xmlrpc_client++ xmlrpc_client xmlrpc++ xmlrpc xmlrpc_util xmlrpc_xmlparse xmlrpc_xmltok)
endif()

# For CM Schema::DBEFile
if(Boost_FOUND)
 set(EXTRA_LIBS ${EXTRA_LIBS} boost_filesystem boost_system)
endif()
if(MAGIC_FOUND)
 set(EXTRA_LIBS ${EXTRA_LIBS} ${MAGIC_LIBRARY})
endif()
if(OPENSSL_VERSION)
 set(EXTRA_LIBS ${EXTRA_LIBS} ${OPENSSL_LIBRARIES})
endif()


message("INCLUDE_DIRECTORIES: ${INCLUDE_DIRECTORIES}")
message("EXTRA_LIBS: ${EXTRA_LIBS}")

#include_directories ("${PROJECT_SOURCE_DIR}/dblayer")
add_subdirectory (schema)
add_subdirectory (dblayer)

if(Qt5Widgets_FOUND)
 add_subdirectory (qdblayer)
endif()

add_subdirectory(tests)

add_subdirectory(apps)
