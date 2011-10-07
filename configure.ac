AC_INIT([dblayer], [0.5.0], [roberto@roccoangeloni.it])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_LANG([C++])
AC_CONFIG_MACRO_DIR([m4])
AC_PROG_LIBTOOL
AC_PROG_CXX
AC_PROG_CC

# Args - Mysql
AC_ARG_WITH([mysql-include-path],
  [AS_HELP_STRING([--with-mysql-include-path],
    [location of the MySQL headers, defaults to /usr/include/mysql])],
  [MYSQL_INCLUDE_PATH="$withval"],
  [MYSQL_INCLUDE_PATH='/usr/include/mysql'])
AC_SUBST([MYSQL_INCLUDE_PATH])
AC_ARG_WITH([mysql-lib-path],
  [AS_HELP_STRING([--with-mysql-lib-path], [location of the MySQL libraries])],
  [MYSQL_LIBS_PATH="-L$withval"],
  [MYSQL_LIBS_PATH=''])
AC_SUBST([MYSQL_LIBS_PATH])

# Args - Xmlrpc-c
AC_ARG_WITH([xmlrpcc-include-path],
  [AS_HELP_STRING([--with-xmlrpcc-include-path],
    [location of the Xmlrpc-c headers, defaults to /usr/include])],
  [XMLRPCC_INCLUDE_PATH="$withval"],
  [XMLRPCC_INCLUDE_PATH='/usr/include'])
AC_SUBST([XMLRPCC_INCLUDE_PATH])
AC_ARG_WITH([xmlrpcc-lib-path],
  [AS_HELP_STRING([--with-xmlrpcc-lib-path], [location of the Xmlrpc-c libraries])],
  [XMLRPCC_LIBS_PATH="-L$withval"],
  [XMLRPCC_LIBS_PATH=''])
AC_SUBST([XMLRPCC_LIBS_PATH])

AC_MSG_NOTICE([])
AC_MSG_NOTICE([Checking available DB drivers...])
AC_MSG_NOTICE([])
AC_CHECK_HEADERS([$MYSQL_INCLUDE_PATH/mysql.h] [$MYSQL_INCLUDE_PATH/mysql/mysql.h],[have_mysql="yes"])
AC_CHECK_HEADERS([odbcinst.h],[have_odbc="yes"])
AC_CHECK_HEADERS([libpq-fe.h] [postgresql/libpq-fe.h],[have_pg="yes"])
AC_CHECK_HEADERS([sqlite3.h],[have_sqlite="yes"])
AC_CHECK_HEADERS([$XMLRPCC_INCLUDE_PATH/xmlrpc-c/base.hpp] [$XMLRPCC_INCLUDE_PATH/xmlrpc-c/base.h],[have_xmlrpc_c="yes"])
AC_MSG_NOTICE([])

if test "x$have_mysql" = xyes
then
 AC_DEFINE([USE_MYSQL],[1],[Enables MySQL specific driver.])
 MYSQL_CFLAGS="-I$MYSQL_INCLUDE_PATH"
 MYSQL_LDFLAGS="$MYSQL_LIBS_PATH -lmysqlclient"
 AC_SUBST(MYSQL_CFLAGS)
 AC_SUBST(MYSQL_LDFLAGS)
fi
if test "x$have_odbc" = "xyes"
then
 AC_DEFINE([USE_ODBCPP],[1],[Enables ODBC specific driver.])
 odbc_lib="odbc"
 AC_CHECK_LIB(odbc,SQLAllocHandle,[odbc_lib="odbc"],[odbc_lib="iodbc"])
 ODBC_LDFLAGS="-l$odbc_lib"
 AC_SUBST(ODBC_LDFLAGS)
fi
if test "x$have_pg" = "xyes"
then
 AC_DEFINE([USE_LIBPQ],[1],[Enables PostgreSQL specific driver.])
 PQ_LDFLAGS="-lpq"
 AC_SUBST(PQ_LDFLAGS)
fi
if test "x$have_sqlite" = "xyes"
then
 AC_DEFINE([USE_LIBSQLITE3],[1],[Enables SQLite3 specific driver.])
 SQLITE_LDFLAGS="-lsqlite3 -ldl -lpthread"
 AC_SUBST(SQLITE_LDFLAGS)
fi
if test "x$have_xmlrpc_c" = "xyes"
then
 AC_DEFINE([USE_LIBXMLRPC],[1],[Enables Xmlrpc-c specific driver.])
 XMLRPCC_CFLAGS="-I$XMLRPCC_INCLUDE_PATH"
 XMLRPCC_LDFLAGS="$XMLRPCC_LIBS_PATH -lxmlrpc_client++ -lxmlrpc_client -lxmlrpc++ -lxmlrpc -lxmlrpc_util -lxmlrpc_xmlparse -lxmlrpc_xmltok"
 AC_SUBST(XMLRPCC_CFLAGS)
 AC_SUBST(XMLRPCC_LDFLAGS)
fi

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
 Makefile
 schema/Makefile
 dblayer/Makefile
 src/Makefile
 doc/Makefile
])
AC_OUTPUT

echo
echo "DBLayer configuration"
echo "  PostgreSQL: $have_pg"
echo "       MySQL: $have_mysql"
echo "        ODBC: $have_odbc"
echo "     SQLite3: $have_sqlite"
echo "    Xmlrpc-c: $have_xmlrpc_c"
echo
