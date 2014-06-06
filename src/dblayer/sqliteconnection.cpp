/***************************************************************************
**	sqliteconnection.cpp  v0.1.1 - 2012.03.19
**	-----------------------------------
**
**	Author:		Roberto Rocco Angeloni.
**	E-mail:		roberto@roccoangeloni.it
**	Comment:	Implementazione di connection e resultset per SQLite
**	To Do:		- SQLiteResultSet::getColumnType: RICONOSCERE SE E' UNA DATA!!!
**	Future:
**	History:
**		v0.1.0 - 2006.05.11 Iniziato lo sviluppo
**		v0.1.1 - 2006.05.14 Completato SQLiteResultSet e SQLiteConnection ad
**				 eccezione di alcuni metodi che non sono supportati in sqlite3
**
** @copyright &copy; 2011-2014 by Roberto Rocco Angeloni <roberto@roccoangeloni.it>
** @license http://opensource.org/licenses/lgpl-3.0.html GNU Lesser General Public License, version 3.0 (LGPLv3)
** @version $Id: sqliteconnection.cpp $
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

#include <cstdio>

#include "sqliteconnection.h"
using namespace DBLayer;

#ifdef USE_LIBSQLITE3


//********************* SQLiteConnection: start.

#ifdef WIN32
SQLiteConnection::SQLiteConnection(string s) : Connection(s) {
#else
SQLiteConnection::SQLiteConnection(string s) : Connection::Connection(s) {
#endif
	this->db = 0;
}
SQLiteConnection::~SQLiteConnection() { this->disconnect(); }

bool SQLiteConnection::connect() {
    this->connected = false;
    int rc;
    const char* tmp = this->connectionString.c_str();
    rc = sqlite3_open( tmp , &this->db);
    this->errorMessage.clear();
    if(rc) {
        sqlite3_close(this->db);
        this->errorMessage.append( sqlite3_errmsg(this->db) );
        return false;
    }
    this->connected = true;
    return true;
}
bool SQLiteConnection::disconnect() {
    int errorCode;
    if( this->db!=0 ) {
        errorCode = sqlite3_close(this->db);

        if( errorCode!=SQLITE_OK ) {
            this->errorMessage.append(
                        "errorCode: "+ DBLayer::integer2string((long)errorCode) +
                        "; msg: " + sqlite3_errmsg(this->db)
                        );
            return false;
        }

        this->db = 0;
    }
    this->connected = false;
    return true;
}
bool SQLiteConnection::reconnect() { return this->disconnect() ? this->connect() : false; }
int SQLiteConnection::sqliteCallback(void *NotUsed, int argc, char **argv, char **azColName) {
    int i;
    SQLiteResultSet* rs = (SQLiteResultSet*) NotUsed;

    for(i=0; i<argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");

    cout << "SQLiteConnection::sqliteCallback: rs = " << rs << endl;
    return 0;
}

ResultSet* SQLiteConnection::exec(const string s) {
#ifdef WIN32
    size_t errorCode;
#else
    int errorCode;
#endif
    const char* query = s.c_str();
    SQLiteResultSet* rs = new SQLiteResultSet();
    const char* zTail = 0;
    sqlite3_stmt* pStmt;

    this->errorMessage.clear();

    errorCode = sqlite3_prepare(
                this->db,               /* Database handle */
                query,       /* SQL statement, UTF-8 encoded */
                s.size(),             /* Length of zSql in bytes. */
                &pStmt,  /* OUT: Statement handle */
                &zTail     /* OUT: Pointer to unused portion of zSql */
                );

    if( errorCode!=SQLITE_OK ) {
        this->errorMessage.append(
                    "errorCode: "+ DBLayer::integer2string((long)errorCode) +
                    "; msg: " + sqlite3_errmsg(this->db)
                    );
        return rs;
    }

    // Preparing metadata
    int nColonne = sqlite3_column_count(pStmt);
    for( int i=0; i<nColonne; i++) {
        const char* nomeColonna = sqlite3_column_name(pStmt,i);
        rs->columnName.push_back( string(nomeColonna) );
        const char* nomeTipo = sqlite3_column_decltype(pStmt,i);
        if(nomeTipo!=0) {
            if( strcmp(nomeTipo,"int")==0 || strcmp(nomeTipo,"integer")==0) {
                rs->columnType.push_back( DBLayer::type_integer );
            } else if( strcmp(nomeTipo,"text")==0 || strncmp(nomeTipo,"varchar",7)==0 ) {
                rs->columnType.push_back( DBLayer::type_string );
            } else if( strcmp(nomeTipo,"float")==0 ) {
                rs->columnType.push_back( DBLayer::type_double );
            } else if( strcmp(nomeTipo,"blob")==0 ) {
                rs->columnType.push_back( DBLayer::type_blob );
            } else {
                cerr << "SQLiteConnection::exec: typeCode \'" << nomeTipo
                     << "\' Unknown!" << endl;
                rs->columnType.push_back( DBLayer::type_blob );
            }
        } else {
            rs->columnType.push_back( DBLayer::type_blob );
        }
    }

    // Fetching first row
    errorCode = sqlite3_step(pStmt);
    if( errorCode!=SQLITE_ROW && errorCode!=SQLITE_DONE ) {
        this->errorMessage.append(
                    "errorCode: "+ DBLayer::integer2string((long)errorCode) +
                    "; msg: " + sqlite3_errmsg(this->db)
                    );
    }

    // Fetching all rows
    while( errorCode==SQLITE_ROW ) {
        for(unsigned int c=0; c<rs->columnName.size(); c++) {
            int sizeDato = sqlite3_column_bytes(pStmt,c);
            if( rs->columnType[c]==DBLayer::type_blob ) {
                int tipoDiDato = sqlite3_column_type(pStmt,c);
                switch( tipoDiDato ) {
                case SQLITE_INTEGER:
                    rs->columnType[c]=DBLayer::type_integer;
                    break;
                case SQLITE_FLOAT:
                    rs->columnType[c]=DBLayer::type_double;
                    break;
                case SQLITE_TEXT:
                    rs->columnType[c]=DBLayer::type_string;
                    break;
                }
            }

            if(sizeDato>0) {
                char* tmpValore = new char[ sqlite3_column_bytes(pStmt,c) * 2 ];
                const unsigned char* tmpSorgente = sqlite3_column_text(pStmt,c);
                sprintf( tmpValore, "%s", tmpSorgente );
                string valore = string( tmpValore );
                rs->righe.push_back( valore );
                delete[] tmpValore;
            } else {
                rs->righe.push_back( string("\\N") );
            }
        }
        errorCode = sqlite3_step(pStmt);
    }
    if( errorCode!=SQLITE_DONE ) {
        this->errorMessage.append(
                    "errorCode: "+ DBLayer::integer2string((long)errorCode) +
                    "; msg: " + sqlite3_errmsg(this->db)
                    );
    }

    errorCode = sqlite3_finalize( pStmt );
    if( errorCode!=SQLITE_OK ) {
        this->errorMessage.append(
                    "errorCode: "+ DBLayer::integer2string((long)errorCode) +
                    "; msg: " + sqlite3_errmsg(this->db)
                    );
    }
    return rs;
}

string SQLiteConnection::escapeString(string s) { return DBLayer::replaceAll(s, "\'", "\'\'"); }

/* Name, type, null, key (PRI,MUL), default */
ColumnDefinitions SQLiteConnection::getColumnsForTable(const string& tablename) {
    ColumnDefinitions ret;
#ifdef WIN32
    size_t errorCode = SQLITE_OK;
#else
    int errorCode = SQLITE_OK;
#endif

    int cols = this->getColumnSize((string*) &tablename);
    for(int c=1; c<=cols && errorCode==SQLITE_OK; c++) {
        StringVector row;
        string columnName = this->getColumnName((string*) &tablename,c);
        char const *pzDataType = 0;
        char const *pzCollSeq = 0;
        int notNull = 0;
        int primaryKey = 0;
        int autoinc = 0;
        try {
            errorCode = sqlite3_table_column_metadata(
                        this->db,
                        0,
                        tablename.c_str(),
                        columnName.c_str(),
                        &pzDataType,    /* OUTPUT: Declared data type */
                        &pzCollSeq,     /* OUTPUT: Collation sequence name */
                        &notNull,       /* OUTPUT: True if NOT NULL constraint exists */
                        &primaryKey,    /* OUTPUT: True if column part of PK */
                        &autoinc        /* OUTPUT: True if column is auto-increment */
                    );
            if( errorCode!=SQLITE_OK ) {
                this->errorMessage.append(
                            "errorCode: "+ DBLayer::integer2string((long)errorCode) +
                            "; msg: " + sqlite3_errmsg(this->db)
                            );
                break;
            }
        } catch(exception e) {
            cerr << "SQLiteConnection::getColumnsForTable: exception = " << e.what() << endl;
        }
        row.push_back(columnName);
        row.push_back(pzDataType);
        row.push_back(notNull>0?"not null":"null");
        row.push_back(primaryKey>0?"PRI":"");

        ret[columnName]=row;
    }

    return ret;
}

int SQLiteConnection::getColumnSize(string* relname) {
    int ret=-1;
#ifdef WIN32
    size_t errorCode;
#else
    int errorCode;
#endif
    string s("select * from ");
    s.append( relname->c_str() );
    const char* query = s.c_str();
    const char* zTail = 0;
    sqlite3_stmt* pStmt;

    this->errorMessage.clear();

    errorCode = sqlite3_prepare(
                this->db,               /* Database handle */
                query,       /* SQL statement, UTF-8 encoded */
                s.size(),             /* Length of zSql in bytes. */
                &pStmt,  /* OUT: Statement handle */
                &zTail     /* OUT: Pointer to unused portion of zSql */
                );

    if( errorCode!=SQLITE_OK ) {
        this->errorMessage.append(
                    "errorCode: "+ DBLayer::integer2string((long)errorCode) +
                    "; msg: " + sqlite3_errmsg(this->db)
                    );
        return ret;
    }

    // Preparing metadata
    int nColonne = sqlite3_column_count(pStmt);
    ret = nColonne;
    errorCode = sqlite3_finalize( pStmt );
    if( errorCode!=SQLITE_OK ) {
        this->errorMessage.append(
                    "errorCode: "+ DBLayer::integer2string((long)errorCode) +
                    "; msg: " + sqlite3_errmsg(this->db)
                    );
    }
    return ret;
}
string SQLiteConnection::getColumnName(string* relname, int column) {
    string ret="";
#ifdef WIN32
    size_t errorCode;
#else
    int errorCode;
#endif
    string s = string("select * from ");
    s.append( relname->c_str() );
    const char* query = s.c_str();
    const char* zTail = 0;
    sqlite3_stmt* pStmt;

    this->errorMessage.clear();

    errorCode = sqlite3_prepare(
                this->db,               /* Database handle */
                query,       /* SQL statement, UTF-8 encoded */
                s.size(),             /* Length of zSql in bytes. */
                &pStmt,  /* OUT: Statement handle */
                &zTail     /* OUT: Pointer to unused portion of zSql */
                );

    if( errorCode!=SQLITE_OK ) {
        this->errorMessage.append(
                    "errorCode: "+ DBLayer::integer2string((long)errorCode) +
                    "; msg: " + sqlite3_errmsg(this->db)
                    );
        return ret;
    }

    // Preparing metadata
    const char* nomeColonna = sqlite3_column_name(pStmt, column-1 );
    ret=string(nomeColonna);

    errorCode = sqlite3_finalize( pStmt );
    if( errorCode!=SQLITE_OK ) {
        this->errorMessage.append(
                    "errorCode: "+ DBLayer::integer2string((long)errorCode) +
                    "; msg: " + sqlite3_errmsg(this->db)
                    );
    }
    return ret;
}
IntegerVector SQLiteConnection::getKeys(string* relname) {
    IntegerVector ret;
    this->errorMessage.append("SQLiteConnection::getKeys: UNSUPPORTED; relname=" + string(relname->c_str()) );
    return ret;
}
IntegerVector SQLiteConnection::getForeignKeys(string* relname) {
    IntegerVector ret;
    this->errorMessage.append("SQLiteConnection::getForeignKeys: UNSUPPORTED; relname=" + string(relname->c_str()));
    return ret;
}
//********************* SQLiteConnection: end.


//********************* SQLiteResultSet: start.

#ifdef WIN32
SQLiteResultSet::SQLiteResultSet() : ResultSet() {
#else
SQLiteResultSet::SQLiteResultSet() : ResultSet::ResultSet() {
#endif
}
SQLiteResultSet::~SQLiteResultSet() {}

int SQLiteResultSet::getColumnSize(int i) { return -i; }

string SQLiteResultSet::toString(string prefix) {
    string ret;
    ret.append(prefix+"<SQLiteResultSet>");
    // Columns
    int nColonne = this->getNumColumns();
    ret.append(prefix+" <Columns>" );
    for( int i=0; i<nColonne; i++) {
        ret.append(prefix+"  <Column ");
        ret.append("position=\'"+DBLayer::integer2string((long)i)+"\' ");
        ret.append("name=\'" + this->getColumnName(i)+"\' ");
        ret.append("type=\'" + this->getColumnType(i)+"\' ");
        ret.append("size=\'" + DBLayer::integer2string((long)this->getColumnSize(i))+"\' ");
        ret.append("/>");
    }
    ret.append(prefix+" </Columns>" );
    // Rows
    ret.append(prefix+" <Rows>");
    int nRighe = this->getNumRows();
    for(int r=0; r<nRighe; r++) {
        ret.append(prefix+"  <Row num=\'"+DBLayer::integer2string((long)r)+"\'>");
        for(int c=0; c<nColonne; c++) {
            string nomeColonna = this->getColumnName(c);
            if (! this->isNull(r,c) ) {
                ret.append(prefix+"   <"+nomeColonna+">");
                ret.append( this->getValue(r,c) );
                ret.append("</"+nomeColonna+">");
            } else {
                ret.append(prefix+"   <"+nomeColonna+" />");
            }
        }
        ret.append(prefix+"  </Row>");
    }
    ret.append(prefix+" </Rows>");

    ret.append(prefix+"</SQLiteResultSet>");
    return ret;
}
//********************* SQLiteResultSet: end.

#endif
