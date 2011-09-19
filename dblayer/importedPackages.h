/***************************************************************************
**                        importedPackages.h  -  description
**                        -------------------
**  begin                : Tue Nov 5 2002
**  email                : roberto_rocco@libero.it
**
** @copyright &copy; 2011 by Roberto Rocco Angeloni <roberto@roccoangeloni.it>
** @license http://opensource.org/licenses/lgpl-3.0.html GNU Lesser General Public License, version 3.0 (LGPLv3)
** @version $Id: importedPackages.h $
** @package rproject::dblayer
**
** Permission to use, copy, modify, and distribute this software for any
** purpose with or without fee is hereby granted, provided that the above
** copyright notice and this permission notice appear in all copies.
**
** THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
** WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
** MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
** ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
** WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
** ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
** OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
****************************************************************************/

#ifndef IMPORTED_PACKAGES_H
#define IMPORTED_PACKAGES_H

#ifndef DLLEXPORT
#if defined(_WIN32) || defined(__CYGWIN__)
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif
#endif

#ifndef DLLIMPORT
#if defined(_WIN32) || defined(__CYGWIN__)
#define DLLIMPORT __declspec(dllimport)
#else
#define DLLIMPORT
#endif
#endif

#ifdef USE_MYSQL
 #ifdef WIN32
  //#include <config-win.h>
  #include <winsock2.h>
  #include <mysql.h>
 #else
  #ifdef __APPLE__
   #include <mysql.h>
  #else
   #include <mysql/mysql.h>
  #endif
 #endif
#endif

# ifdef USE_ODBCPP
 #ifdef __WIN32
  #include <windef.h>
  //#include <winnt.h>
  #include <mbstring.h>
  #include <windows.h>
 #else
  #include <sqlext.h>
  #include <odbcinst.h>
 #endif
 // ODBC++
 //#include <odbc++/connection.h>
 //#include <odbc++/drivermanager.h>
 //#include <odbc++/preparedstatement.h>
 //#include <odbc++/resultset.h>
 //#include <odbc++/resultsetmetadata.h>
 //#include <odbc++/types.h>
 //using namespace odbc;
# endif

#ifdef USE_LIBPQ
 #ifdef WIN32
  #include <libpq-fe.h>
  #include <sql3types.h>
 #else
  #ifdef HAVE_LIBPQ_FE_H
   #include <libpq-fe.h>
  #else
   #include <postgresql/libpq-fe.h>
  #endif
  //#include <pgsql/libpq-fe.h>
  //#include <pgsql/sql3types.h>
 #endif
# endif

#ifdef USE_LIBSQLITE3
#ifdef WIN32
//#include <windef.h>
#endif
#include <sqlite3.h>
#endif

#ifdef USE_LIBXMLRPC
#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/client.hpp>
#include <xmlrpc-c/client_simple.hpp>
#endif

// STL
#include <algorithm>
#include <iostream>
#include <map>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

#endif
