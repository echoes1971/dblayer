/***************************************************************************
**	odbcconnection.cpp  v0.1.0 - 2006.05.21
**	---------------------------------------
**
**	Author:		Roberto Rocco Angeloni.
**	email:		roberto@roccoangeloni.it
**	Comment:	Implementazione di connection e resultset per ODBC
**	To Do:		- ...
**	Future:
**	History:
**		v0.1.0 - 2006.05.21 Iniziato lo sviluppo
**
** @copyright &copy; 2011 by Roberto Rocco Angeloni <roberto@roccoangeloni.it>
** @license http://opensource.org/licenses/lgpl-3.0.html GNU Lesser General Public License, version 3.0 (LGPLv3)
** @version $Id: odbcconnection.cpp $
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
#ifdef WIN32
#include "stdafx.h"
using namespace System;
#else
#include <config.h>
#endif

#include "odbcconnection.h"
using namespace DBLayer;

#ifdef USE_ODBCPP

//********************* ODBCConnection: inizio.
#ifdef WIN32
ODBCConnection::ODBCConnection(string s) : Connection(s) {
#else
ODBCConnection::ODBCConnection(string s) : Connection::Connection(s) {
#endif
	this->connectionEstablished = true;
	SQLAllocEnv(&henv);
#ifdef __WIN32
	_mbscpy(chr_ds_name,(const unsigned char *) s.c_str() );
#else
	strcpy(chr_ds_name, s.c_str() );
#endif
}
ODBCConnection::~ODBCConnection() {
	this->disconnect();
	SQLFreeEnv(henv);
}

bool ODBCConnection::isRCSuccessful() { return (rc==SQL_SUCCESS) || (rc==SQL_SUCCESS_WITH_INFO); }
void ODBCConnection::saveErrorMessage() {
    unsigned char szSQLSTATE[10];
#ifdef __APPLE__
    SQLINTEGER nErr;
#else
 #if (ODBCVER <= 0x0351)
    SQLINTEGER nErr;
 #else
    SDWORD nErr;
#endif
#endif
    unsigned char msg[SQL_MAX_MESSAGE_LENGTH+1];
    SWORD cbmsg;
    unsigned char szData[MAX_DATA];

    this->errorMessage.clear();
    while(SQLError(0,0,hstmt,szSQLSTATE,&nErr,msg,sizeof(msg),&cbmsg) == SQL_SUCCESS) {
#ifdef __WIN32
        wsprintf((char *)szData,
                "Error:\nSQLSTATE=%s,Native	error=%ld,msg='%s'",
                szSQLSTATE,nErr,msg);
#else
        sprintf((char *)szData,
                "Error:\nSQLSTATE=%s,Native	error=%ld,msg='%s'",
                szSQLSTATE,(long int) nErr,msg);
#endif
        this->errorMessage.append( (const char *) szData );
    }
}

bool ODBCConnection::connect() {
    bool ret = true;
    SQLAllocConnect(henv, &hdbc);
    rc=SQLConnect(hdbc,(SQLCHAR*) chr_ds_name,SQL_NTS,NULL,0,NULL,0);
    if( !this->isRCSuccessful() ) {
        this->saveErrorMessage();
        ret = false;
    };
    this->connectionEstablished = ret;

    this->dbmsName.clear();
    SQLCHAR dbmsNameChar[MAX_DATA];
    SQLSMALLINT dbmsName_length=-1;
    SQLGetInfo( hdbc, SQL_DBMS_NAME, &dbmsNameChar, MAX_DATA, &dbmsName_length);
    this->dbmsName.append( (const char*) dbmsNameChar );

    return ret;
}
bool ODBCConnection::disconnect() {
    if(this->connectionEstablished) {
        SQLFreeConnect(hdbc);
        this->connectionEstablished = false;
    };
    return true;
}

string ODBCConnection::decodeType(SQLSMALLINT mytype) {
    switch(mytype) {
    case SQL_BIT:
        return DBLayer::type_boolean;
    case SQL_DECIMAL:
    case SQL_NUMERIC:
    case SQL_SMALLINT:
    case SQL_INTEGER:
    case SQL_BIGINT:
    case SQL_TINYINT:
        return DBLayer::type_integer;
    case SQL_REAL:
    case SQL_FLOAT:
    case SQL_DOUBLE:
        return DBLayer::type_double;
    case SQL_CHAR:
    case SQL_VARCHAR:
    case SQL_LONGVARCHAR:
#ifdef __WIN32
    case SQL_WCHAR:
    case SQL_WVARCHAR:
    case SQL_WLONGVARCHAR:
#endif
        return DBLayer::type_string;
    case SQL_TYPE_DATE:
    case SQL_TYPE_TIME:
    case SQL_TYPE_TIMESTAMP:
    //case SQL_TYPE_UTCDATETIME:
    //case SQL_TYPE_UTCTIME:
    case 9:
    case 11:
        return DBLayer::type_datetime;
    case SQL_BINARY:
    case SQL_VARBINARY:
    case SQL_LONGVARBINARY:
        return DBLayer::type_blob;
    default:
        return DBLayer::type_blob;
    }
}

string ODBCConnection::quoteDate(string s) { return this->dbmsName == "ACCESS" ? "#"+s+"#" : "'"+s+"'"; }

ResultSet* ODBCConnection::exec(const string s) {
    ODBCResultSet* rs = new ODBCResultSet();
    unsigned char* cmdstr = (unsigned char*) s.c_str();

    rc=SQLAllocStmt(hdbc,&hstmt);
    if( !this->isRCSuccessful() ) {
        this->saveErrorMessage();
        return rs;
    };

    rc=SQLExecDirect(hstmt,cmdstr,SQL_NTS);
    if( !this->isRCSuccessful() ) {
        saveErrorMessage();
        return rs;
    }
    // Preparo i metadati
    SQLSMALLINT numColumns=-1;
    SQLNumResultCols(hstmt, &numColumns);
    for(int c=1; c<=numColumns; c++) {
        SQLCHAR columnName[MAX_DATA];
        SQLSMALLINT columnName_size=-1;
        SQLSMALLINT columnType=-1;
        SQLULEN columnSize=0;
        SQLSMALLINT decimalDigits=-1;
        SQLSMALLINT nullable=-1;
        SQLDescribeCol( hstmt, c,
                        (SQLCHAR*) &columnName, sizeof(columnName), &columnName_size,
                        &columnType, &columnSize, &decimalDigits, &nullable
                        );
        if(columnName_size<=0)	columnName[0] = '\0';
        string columnTypeString = this->decodeType(columnType);
        rs->columnName.push_back( string((const char*) columnName) );
        rs->columnType.push_back( columnTypeString );
        rs->columnSize.push_back( columnSize );
    };

    // Leggo tutte le righe
    unsigned char szData[MAX_DATA];
    SQLLEN cbData;     // Output length of data
    for (rc=SQLFetch(hstmt); rc == SQL_SUCCESS; rc=SQLFetch(hstmt)) {
        SQLNumResultCols(hstmt, &numColumns);
        for(int c=1; c<=numColumns; c++) {
            szData[0]='\0';
            SQLGetData(hstmt,c,SQL_C_CHAR,szData,sizeof(szData),&cbData);
            if(cbData>=0) {
                string tmp = string( (const char*) szData );
                while(cbData>=MAX_DATA) {
                    SQLGetData(hstmt,c,SQL_C_CHAR,szData,sizeof(szData),&cbData);
                    tmp.append( (const char*) szData );
                }
                rs->righe.push_back( tmp );
            } else {
                rs->righe.push_back( string("\\N") );
            }
        };
    };
    SQLFreeStmt(hstmt,SQL_DROP);
    return rs;
}
//********************* ODBCConnection: fine.



//********************* ODBCResultSet: inizio.
ODBCResultSet::ODBCResultSet() : ResultSet::ResultSet() {}
ODBCResultSet::~ODBCResultSet() {}
//********************* ODBCResultSet: fine.

#endif
