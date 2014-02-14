# - Find XmlrpcC
# Find the XmlrpcC includes and client library
# This module defines
#  XMLRPCC_INCLUDE_DIR, where to find XMLRPCC.h
#  XMLRPCC_LIBRARIES, the libraries needed to use XMLRPCC.
#  XMLRPCC_FOUND, If false, do not try to use XmlrpcC.

# Copyright (c) 2006, Jaroslaw Staniek, <js@iidea.pl>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.


if (XMLRPCC_INCLUDE_DIR AND XMLRPCC_LIBRARIES)
  # Already in cache, be silent
  set(XmlrpcC_FIND_QUIETLY TRUE)
endif (XMLRPCC_INCLUDE_DIR AND XMLRPCC_LIBRARIES)


find_path(XMLRPCC_INCLUDE_DIR xmlrpc-c
   /usr/include/
   /usr/local/include/
   /usr/include/
   /opt/include/
   /opt/local/include/
   /opt/include/
)

message("XMLRPCC_INCLUDE_DIR: ${XMLRPCC_INCLUDE_DIR}")

find_library(XMLRPCC_TMP_LIBRARIES NAMES xmlrpc_client++ xmlrpc_client xmlrpc++ xmlrpc xmlrpc_util xmlrpc_xmlparse xmlrpc_xmltok)

message("XMLRPCC_TMP_LIBRARIES: ${XMLRPCC_TMP_LIBRARIES}")

if(XMLRPCC_INCLUDE_DIR AND XMLRPCC_TMP_LIBRARIES)
  set(XMLRPCC_FOUND 1)
  set(XMLRPCC_LIBRARIES xmlrpc_client++ xmlrpc_client xmlrpc++ xmlrpc xmlrpc_util xmlrpc_xmlparse xmlrpc_xmltok)
endif()
message("XMLRPCC_LIBRARIES: ${XMLRPCC_LIBRARIES}")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(XmlrpcC DEFAULT_MSG
                                  XMLRPCC_INCLUDE_DIR XMLRPCC_LIBRARIES )

mark_as_advanced(XMLRPCC_FOUND XMLRPCC_INCLUDE_DIR XMLRPCC_LIBRARIES)

