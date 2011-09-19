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
#include <config.h>

#include "odbcconnection.h"
using namespace DBLayer;

#ifdef USE_ODBCPP

//********************* ODBCConnection: inizio.
#ifdef WIN32
ODBCConnection::ODBCConnection(string s) : Connection(s) {
#else
ODBCConnection::ODBCConnection(string s) : Connection::Connection(s) {
#endif
//	SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv1);
	this->connectionEstablished = true;
	SQLAllocEnv(&henv);
#ifdef __WIN32
	_mbscpy(chr_ds_name,(const unsigned char *) s.c_str() );
#else
	strcpy(chr_ds_name, s.c_str() );
#endif
}
ODBCConnection::~ODBCConnection() {
//	cout << "ODBCConnection::~ODBCConnection: inizio." << endl;
	this->disconnect();
	SQLFreeEnv(henv);
//	cout << "ODBCConnection::~ODBCConnection: fine." << endl;
}

bool ODBCConnection::isRCSuccessful() { return (rc==SQL_SUCCESS) || (rc==SQL_SUCCESS_WITH_INFO); }
void ODBCConnection::saveErrorMessage() {
    unsigned char szSQLSTATE[10];
#ifdef __APPLE__
    SQLINTEGER nErr;
#else
    SDWORD nErr;
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
//		printf((char *)szData,
//			"Error:\nSQLSTATE=%s,Native	error=%ld,msg='%s'",
//			szSQLSTATE,nErr,msg);
        this->errorMessage.append( (const char *) szData );
    }
//		printf((char *)szData,
//			"Error:\nSQLSTATE=%s,Native	error=%ld,msg='%s'",
//			szSQLSTATE,nErr,msg);
}

bool ODBCConnection::connect() {
//	cout << "ODBCConnection::connect: inizio." << endl;
    bool ret = true;
    SQLAllocConnect(henv, &hdbc);
    rc=SQLConnect(hdbc,(SQLCHAR*) chr_ds_name,SQL_NTS,NULL,0,NULL,0);
    if( !this->isRCSuccessful() ) {
        printf("ODBCConnection::connect: unable to connect - rc=\'%d\'\n",rc);
//		SQLFreeEnv(henv);
//		SQLFreeConnect(hdbc);
        this->saveErrorMessage();
        ret = false;
    };
//	rc=SQLAllocStmt(hdbc,&hstmt);
//	if( !this->isRCSuccessful() ) {
//		printf("ODBCConnection::connect: unable to allocate stmt - rc=\'%d\'\n",rc);
//		this->saveErrorMessage();
//		ret = false;
//	};
//	cout << "ODBCConnection::connect: fine." << endl;
    this->connectionEstablished = ret;

    this->dbmsName.clear();
    SQLCHAR dbmsNameChar[MAX_DATA];
    SQLSMALLINT dbmsName_length=-1;
    SQLGetInfo( hdbc, SQL_DBMS_NAME, &dbmsNameChar, MAX_DATA, &dbmsName_length);
    printf("ODBCConnection::connect: dbmsName=\'%s\'\n",dbmsNameChar);
    this->dbmsName.append( (const char*) dbmsNameChar );

    return ret;
}
bool ODBCConnection::disconnect() {
//	cout << "ODBCConnection::disconnect: inizio." << endl;
    if(this->connectionEstablished) {
//		SQLFreeStmt(hstmt,SQL_DROP);
        SQLFreeConnect(hdbc);
        this->connectionEstablished = false;
    };
//	cout << "ODBCConnection::disconnect: fine." << endl;
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
        cout << "ODBCConnection::decodeType: ERROR Type " << mytype << " not recognized!!!" << endl;
        return DBLayer::type_blob;
    }
}

string ODBCConnection::quoteDate(string s) { return this->dbmsName == "ACCESS" ? "#"+s+"#" : "'"+s+"'"; }

ResultSet* ODBCConnection::exec(const string s) {
    ODBCResultSet* rs = new ODBCResultSet();
    unsigned char* cmdstr = (unsigned char*) s.c_str();

    rc=SQLAllocStmt(hdbc,&hstmt);
    if( !this->isRCSuccessful() ) {
        printf("ODBCConnection::connect: unable to allocate stmt - rc=\'%d\'\n",rc);
        this->saveErrorMessage();
        return rs;
    };

    //	printf("ODBCConnection::exec: cmdstr=\'%s\'\n",cmdstr);
    rc=SQLExecDirect(hstmt,cmdstr,SQL_NTS);
    //	printf("ODBCConnection::exec: rc=\'%d\'\n",rc);
    if( !this->isRCSuccessful() ) {
        saveErrorMessage();
        cout << "ODBCConnection::exec: ERROR - msg = " << this->errorMessage << endl;
        cout << "ODBCConnection::exec: ERROR -   s = " << s << endl;
        return rs;
    }
    // Preparo i metadati
    SQLSMALLINT numColumns=-1;
    SQLNumResultCols(hstmt, &numColumns);
    //	printf("ODBCConnection::exec: numColumns=%d\n",numColumns);
    for(int c=1; c<=numColumns; c++) {
        SQLCHAR columnName[MAX_DATA];
        SQLSMALLINT columnName_size=-1;
        SQLSMALLINT columnType=-1;
        // 2011.09.04: start.
        SQLULEN columnSize=0;
//        SQLUINTEGER columnSize=0;
        // 2011.09.04: end.
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
        //printf("ODBCConnection::exec: columnName=\'%s\' size=%d type=\'%s\'(%d) decimalDigits=%d nullable=%d\n",
        //	columnName, columnSize, columnTypeString.c_str(), columnType, decimalDigits, nullable
        //);
    };

    // Leggo tutte le righe
    unsigned char szData[MAX_DATA];
    // 2011.09.04: start.
    SQLLEN cbData;     // Output length of data
//    SDWORD cbData;     // Output length of data
    // 2011.09.04: end.
    for (rc=SQLFetch(hstmt); rc == SQL_SUCCESS; rc=SQLFetch(hstmt)) {
        SQLNumResultCols(hstmt, &numColumns);
        //		printf("ODBCConnection::exec: numColumns=%d\n",numColumns);
        //		printf("ODBCConnection::exec: ( ");
        for(int c=1; c<=numColumns; c++) {
            szData[0]='\0';
            SQLGetData(hstmt,c,SQL_C_CHAR,szData,sizeof(szData),&cbData);
            if(cbData>=0) {
                //				printf("ODBCConnection::exec: szData=\'%s\'(%d)\n",szData,cbData);
                string tmp = string( (const char*) szData );
                while(cbData>=MAX_DATA) {
                    SQLGetData(hstmt,c,SQL_C_CHAR,szData,sizeof(szData),&cbData);
                    //					printf("ODBCConnection::exec: szData=\'%s\'(%d)\n",szData,cbData);
                    tmp.append( (const char*) szData );
                }
                rs->righe.push_back( tmp );
            } else {
                rs->righe.push_back( string("\\N") );
                //				printf("\\N");
            }
            //			if(c!=numColumns)	printf(",");
        };
        //		printf(" )\n");
    };

    SQLFreeStmt(hstmt,SQL_DROP);

    return rs;
}
//********************* ODBCConnection: fine.



//********************* ODBCResultSet: inizio.
ODBCResultSet::ODBCResultSet() : ResultSet::ResultSet() {}
ODBCResultSet::~ODBCResultSet() {
//	cout << "ODBCResultSet::~ODBCResultSet: inizio." << endl;
//	cout << "ODBCResultSet::~ODBCResultSet: fine." << endl;
}
//********************* ODBCResultSet: fine.

#endif
