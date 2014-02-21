# - Try to find Sqlite3
# Once done this will define
#
#  SQLITE3_FOUND - system has Sqlite3
#  SQLITE3_INCLUDE_DIRS - the Sqlite3 include directory
#  SQLITE3_LIBRARIES - Link these to use Sqlite3
#  SQLITE3_DEFINITIONS - Compiler switches required for using Sqlite3
#
#  Copyright (c) 2008 Andreas Schneider <mail@cynapses.org>
#
#  Redistribution and use is allowed according to the terms of the New
#  BSD license.
#  For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#


if (SQLITE3_LIBRARIES AND SQLITE3_INCLUDE_DIRS)
  # in cache already
  set(SQLITE3_FOUND TRUE)
else()
  if(APPLE)
    message("APPLE!!!!!!!!!!!!!!!!!!!!!!!!!")
    find_path(SQLITE3_INCLUDE_DIR
        NAMES
        sqlite3.h
        PATHS
        /opt/local/include
        /usr/include
        /usr/local/include
        /sw/include
        NO_DEFAULT_PATH
    )
    find_library(SQLITE3_LIBRARY
        NAMES
        sqlite3
        PATHS
        /opt/local/lib
        /usr/lib
        /usr/local/lib
        /sw/lib
        NO_DEFAULT_PATH
    )
  else()
    # use pkg-config to get the directories and then use these values
    # in the FIND_PATH() and FIND_LIBRARY() calls
    find_package(PkgConfig)
    if(PKG_CONFIG_FOUND)
      pkg_check_modules(_SQLITE3 REQUIRED sqlite3)
      set(SQLITE3_DEFINITIONS ${_SQLITE3_CFLAGS})
      #set(SQLITE3_DEFINITIONS ${_SQLITE3_CFLAGS_OTHER})
      message("_SQLITE3_LIBRARIES: ${_SQLITE3_LIBRARIES}")
      message("_SQLITE3_INCLUDE_DIRS: ${_SQLITE3_INCLUDE_DIRS}")
      message("_SQLITE3_INCLUDEDIR: ${_SQLITE3_INCLUDEDIR}")
      message("_SQLITE3_CFLAGS: ${_SQLITE3_CFLAGS}")
      message("_SQLITE3_CFLAGS_OTHER: ${_SQLITE3_CFLAGS_OTHER}")
      message("_SQLITE3_LDFLAGS: ${_SQLITE3_LDFLAGS}")
      message("_SQLITE3_LDFLAGS_OTHER: ${_SQLITE3_LDFLAGS_OTHER}")
    endif()

    find_path(SQLITE3_INCLUDE_DIR
      NAMES
        sqlite3.h
      PATHS
        ${_SQLITE3_INCLUDEDIR}
        /usr/include
        /usr/local/include
        /sw/include
    )
    find_library(SQLITE3_LIBRARY
      NAMES
        sqlite3
      PATHS
        ${_SQLITE3_LIBRARIES}
        /usr/lib
        /usr/local/lib
        /sw/lib
    )
  endif()
  message("SQLITE3_INCLUDE_DIR: ${SQLITE3_INCLUDE_DIR}")
  message("SQLITE3_LIBRARY: ${SQLITE3_LIBRARY}")

  if(SQLITE3_LIBRARY)
    set(SQLITE3_FOUND TRUE)
  endif()

  set(SQLITE3_INCLUDE_DIRS ${SQLITE3_INCLUDE_DIR} )

  if(SQLITE3_FOUND)
    set(SQLITE3_LIBRARIES
      ${SQLITE3_LIBRARIES}
      ${SQLITE3_LIBRARY}
    )
  endif()

  if(SQLITE3_INCLUDE_DIRS AND SQLITE3_LIBRARIES)
     set(SQLITE3_FOUND TRUE)
  endif()

  if(SQLITE3_FOUND)
    if(NOT Sqlite3_FIND_QUIETLY)
      message(STATUS "Found Sqlite3: ${SQLITE3_LIBRARIES}")
    endif()
  else()
    if (Sqlite3_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find Sqlite3")
    endif (Sqlite3_FIND_REQUIRED)
  endif()

  # show the SQLITE3_INCLUDE_DIRS and SQLITE3_LIBRARIES variables only in the advanced view
  mark_as_advanced(SQLITE3_INCLUDE_DIRS SQLITE3_LIBRARIES)

endif()

