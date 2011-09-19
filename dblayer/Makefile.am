CURRENT=1
REVISION=0
AGE=0

CPPFLAGS = -Wall -I$(top_srcdir) $(MYSQL_CFLAGS)
LDFLAGS = -L../schema -lschema
lib_LTLIBRARIES = libdblayer.la
libdblayer_ladir = $(includedir)/dblayer
libdblayer_la_LDFLAGS = -version-info $(CURRENT):$(REVISION):$(AGE) -L../schema -lschema $(MYSQL_LDFLAGS) $(ODBC_LDFLAGS) $(PQ_LDFLAGS) $(SQLITE_LDFLAGS) $(XMLRPCC_LDFLAGS)
libdblayer_la_SOURCES = \
	booleanfield.cpp floatfield.cpp integerfield.cpp stringfield.cpp datefield.cpp dbfield.cpp \
	dbconnection.cpp mysqlconnection.cpp sqliteconnection.cpp odbcconnection.cpp xmlrpcconnection.cpp \
	dbentity.cpp dblayer.cpp dbefactory.cpp dbmgr.cpp foreignkey.cpp

libdblayer_la_HEADERS = booleanfield.h dbconnection.h dbentity.h dblayer.h floatfield.h importedPackages.h \
        mysqlconnection.h sqliteconnection.h xmlrpcconnection.h datefield.h dbefactory.h \
	dbfield.h dbmgr.h foreignkey.h integerfield.h odbcconnection.h stringfield.h
