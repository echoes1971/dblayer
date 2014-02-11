cmake_minimum_required(VERSION 2.8)
project(DBLayer)
# The version number.
set (Tutorial_VERSION_MAJOR 0)
set (Tutorial_VERSION_MINOR 5)

message("PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")

include_directories ("${PROJECT_SOURCE_DIR}")

message("INCLUDE_DIRECTORIES: ${INCLUDE_DIRECTORIES}")

#include_directories ("${PROJECT_SOURCE_DIR}/dblayer")
add_subdirectory (schema)
add_subdirectory (dblayer)
add_subdirectory (src)


