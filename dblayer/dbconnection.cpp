/***************************************************************************
**	dbconnection.cpp  v0.1.2 - 2006.05.22
**	-------------------------------------
**
**	Author:		Roberto Rocco Angeloni.
**	email:		roberto@roccoangeloni.it
**	Comment:	Implementazione di connection e resultset
**	To Do:		- getForeignKeys
**	Future:
**	History:
**		v0.1.0 - 2006.05.09 Prima versione stabile
**		v0.1.1 - 2006.05.13 Modificata la createConnection, ora e' possibile
**					passare una stringa di connessione del tipo
**					"dblayer:sqlite3:c:\tmp\test.db"
**					"dblayer:pg:host=localhost dbname=roberto user=roberto password="
**					:-) :-) :-) :-) :-) :-) :-) :-) :-) :-) :-) :-) :-) :-)
**		v0.1.2 - 2006.05.22 Implementato ResultSet come SQLiteResultSet, ovvero con
**					l'utilizzo di vettori di stringhe per memorizzare i risultati
**
** @copyright &copy; 2011 by Roberto Rocco Angeloni <roberto@roccoangeloni.it>
** @license http://opensource.org/licenses/lgpl-3.0.html GNU Lesser General Public License, version 3.0 (LGPLv3)
** @version $Id: dbconnection.cpp $
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
#ifdef USE_LIBPQ
 #include "dbconnection.h"
#endif
#include "mysqlconnection.h"
#include "odbcconnection.h"
#include "sqliteconnection.h"
#include "xmlrpcconnection.h"
using namespace DBLayer;

ConnectionBuildersMap DBLayer::connectionBuilders;// = ConnectionBuildersMap();

Connection* DBLayer::createConnection(string s) {
    string myConnString = s;
    string myDBType = db_type;

#ifdef __i386__
    unsigned int indice = (unsigned int) s.find("dblayer:");
#else
    unsigned long indice = s.find("dblayer:");
#endif
    if( indice != string::npos ) {
        unsigned int indicePrefixInizio = (unsigned int) indice+8;
#ifdef __i386__
        unsigned int indicePrefixFine = (unsigned int) s.find(":", indicePrefixInizio);
#else
        unsigned long indicePrefixFine = s.find(":", indicePrefixInizio);
#endif
        string prefix = s.substr(indicePrefixInizio, indicePrefixFine-indicePrefixInizio );
        string connString = s.substr(indicePrefixFine+1);

        myDBType = prefix;
        myConnString = connString;
    }
    if (myDBType == db_mysql) {
#ifdef USE_MYSQL
        return new MySQLConnection(myConnString);
#endif
    } else if (myDBType == db_odbc) {
#ifdef USE_ODBCPP
        return new ODBCConnection(myConnString);
#endif
    } else if (myDBType == db_pg) {
#ifdef USE_LIBPQ
        return new PGConnection(myConnString);
#endif
    } else if (myDBType == db_sqlite) {
#ifdef USE_LIBSQLITE3
        return new SQLiteConnection(myConnString);
#endif
    } else if (myDBType == db_xmlrpc) {
#ifdef USE_LIBXMLRPC
        return new XmlrpcConnection(myConnString);
#endif
    };
    ConnectionBuildersMap::iterator it;
    it = DBLayer::connectionBuilders.find(myDBType);
    if(it!=DBLayer::connectionBuilders.end())
        return (*it->second)(myConnString);
    cerr << "DBLayer::createConnection: type '" << myDBType << "' not found." << endl;
    return new Connection(s);
}
void DBLayer::registerConnectionType(string prefix, Connection* (*myBuilder)(string s)) {
    DBLayer::connectionBuilders[prefix] = myBuilder;
}

//********************** Connection

Connection::Connection(string s) {
    this->connectionString = s;
    this->errorMessage.clear();
    this->connected = false;
}

Connection::~Connection() { this->disconnect(); }

bool Connection::connect() {
    this->connected = false;
    return false;
}

bool Connection::disconnect() {
    this->connected = false;
    return true;
}

bool Connection::isConnected() { return this->connected; }

ResultSet* Connection::login(string user, string pwd) { return new ResultSet(); }

string Connection::getFormSchema(string language) { return "Connection::getFormSchema: NOT SUPPORTED - " + language; }
string Connection::getDBSchema(string language) { return "Connection::getDBSchema: NOT SUPPORTED - " + language; }
string Connection::getSchemaName() { return "Connection::getSchemaName: NOT SUPPORTED"; }

ResultSet* Connection::exec(const string s) { return new ResultSet(); }

string Connection::getErrorMessage() { return this->errorMessage; }
bool Connection::hasErrors() { return this->getErrorMessage().length() > 0; }

void Connection::setVerbose(bool b) { this->verbose=b; }
bool Connection::isVerbose() { return this->verbose; }

bool Connection::flush() { return true; }

bool Connection::reconnect() { return false; }

string* Connection::escapeString(string* s) { return s; }
string Connection::escapeString(string s) { return DBLayer::replaceAll(s, "\'", "\'\'"); }
string Connection::quoteDate(string s) { return "'"+DBLayer::replaceAll(s, " 00:00:00", "")+"'"; }

ColumnDefinitions Connection::getColumnsForTable(const string& tablename) {
    ColumnDefinitions ret;
    return ret;
}

int Connection::getColumnSize(string* relname) { return -1; }
string Connection::getColumnName(string* relname, int column) { return string(""); }

IntegerVector Connection::getKeys(string* relname) {
    IntegerVector ret;
    return ret;
}
IntegerVector Connection::getForeignKeys(string* relname) {
    IntegerVector ret;
    return ret;
}

StringVector Connection::getKeysNames(string* relname) {
    StringVector ret;
    IntegerVector chiavi = this->getKeys(relname);
    for(unsigned int i=0; i<chiavi.size(); i++) {
        ret.push_back( this->getColumnName(relname, chiavi[i] ) );
    }
    return ret;
}
// **************** Proxy Connections *********************
// The proxy connections are used by DBMgr to execute the following methods
bool Connection::isProxy() { return false; }
DBEntity* Connection::Insert(DBEntity *dbe) { return dbe; }
DBEntity* Connection::Update(DBEntity *dbe) { return dbe; }
DBEntity* Connection::Delete(DBEntity *dbe) { return dbe; }
DBEntityVector* Connection::Select(DBEntity* dbe, const string* tableName, const string* searchString) { return (DBEntityVector*) (0 & (long)dbe & (long)tableName & (long)searchString); }
DBEntityVector* Connection::Search(DBEntity* dbe, bool uselike, bool caseSensitive, const string* orderBy ) { return (DBEntityVector*) (0 & (long)dbe & (long)uselike & (long)caseSensitive & (long)orderBy); }
// **************** Proxy Connections: end. *********************


//********************** PGConnection

#ifdef USE_LIBPQ

PGConnection::PGConnection(string s) : Connection::Connection(s) { this->conn = 0; }
PGConnection::~PGConnection() { this->disconnect(); }

bool PGConnection::connect() {
    if(this->conn!=0) return true;
    this->conn = PQconnectdb( this->connectionString.c_str() );
    if( PQstatus(this->conn) == CONNECTION_OK ) {
        this->errorMessage.clear();
        this->connected = true;
    } else {
        this->errorMessage = string( PQerrorMessage(this->conn) );
        this->connected = false;
    }
    return this->connected;
}

bool PGConnection::disconnect() {
    if( this->isConnected() ) {
        PQfinish(this->conn);
        this->conn=0;
    }
    this->connected = false;
    return true;
}

ResultSet* PGConnection::exec(const string s) {
    PGresult* res = PQexec(this->conn, s.c_str() );
    if (PQresultStatus(res) != PGRES_TUPLES_OK) { //PGRES_COMMAND_OK) {
        this->errorMessage = string( PQresultErrorMessage(res) );//PQerrorMessage(this->conn) );
    //PQclear(res);	RRA: lascio la distruzione del PGResultSet al chiamante
    PGResultSet* ret = new PGResultSet(res);
        return ret;
    } else {
        this->errorMessage.clear();
        return new PGResultSet( res );
    }
}

string PGConnection::escapeString(string s) {
    char* to=new char[s.length()*3];
    PQescapeString(to, s.c_str(), s.size() );
    string ret(to);
    delete[] to;
    return ret;
}

bool PGConnection::flush() {
    PQflush( this->conn );
    return true;
}

bool PGConnection::reconnect() {
    PQreset( this->conn );
    return true;
}

/* Name, type, null, key (PRI,MUL), default */
/*
select a.*, b.adsrc
  from pg_attribute a join pg_class c on (a.attrelid=c.oid)
       left join pg_attrdef b on (a.attnum=b.adnum and a.attrelid=b.adrelid)
 where relname='rra_users' and attnum>0
 order by a.attnum
*/
ColumnDefinitions PGConnection::getColumnsForTable(const string& tablename) {
    ColumnDefinitions ret;
    DBLayer::ResultSet* res;

    string myquery;
    myquery.append("select a.attname as nome, a.atttypid as tipo, a.attnotnull as nullo, a.atthasdef as hasdefault, b.adsrc as valoredefault")
            .append("  from pg_attribute a join pg_class c on (a.attrelid=c.oid)")
            .append("       left join pg_attrdef b on (a.attnum=b.adnum and a.attrelid=b.adrelid)")
            .append(" where c.relname=\'").append( tablename.c_str() ).append("\' and a.attnum>0")
            .append(" order by a.attnum");
    res = this->exec(myquery);
    //cout << res->toString() << endl;

    if( !this->hasErrors() ) {
        for(int r=0; r<res->getNumRows(); r++) {
            StringVector row;
            int c=0;
            row.push_back( res->getValue(r,c++) );
            row.push_back( PGConnection::pgtype2string( atoi( res->getValue(r,c++).c_str() ) ) );
            if( res->getValue(r,c++)=="t" )
                row.push_back("not null");
            else
                row.push_back("");
            if(res->getValue(r,c++)=="t") // has default?
                //TODO: eliminare le parti in piu di postgresql
                row.push_back( res->getValue(r,c++) );
            else
                row.push_back( "\\N" );
            ret[ res->getValue(r,0) ] = row;
        }
    } else {
        //cout << "Errori: " << this->getErrorMessage() << endl;
    }

    delete res;
    return ret;
}

int PGConnection::getColumnSize(string* relname) {
    int ret = -1;
    DBLayer::ResultSet* res;

    string myquery;
    myquery.append("select count(*) as numero")
            .append("  from pg_attribute a join pg_class c on (a.attrelid=c.oid)")
            .append(" where relname='").append( relname->c_str() ).append("' and attnum>0");
    res = this->exec(myquery);

    if( !this->hasErrors() ) {
        string tmp = res->getValue(0,0);
        ret = atoi( tmp.c_str() );
    }
    delete res;
    return ret;
}

string PGConnection::getColumnName(string* relname, int column) {
    string ret = string("");
    DBLayer::ResultSet* res;
    char numColonna[50];
    sprintf( numColonna, "%d", column );

    string myquery;
    myquery.append("select a.attname as numero")
            .append("  from pg_attribute a join pg_class c on (a.attrelid=c.oid)")
            .append(" where relname=\'").append( relname->c_str() ).append("\' and attnum=").append(string(numColonna));
    res = this->exec(myquery);

    if( !this->hasErrors() ) {
        ret = res->getValue(0,0);
    }
    delete res;
    return ret;
}

IntegerVector PGConnection::getKeys(string* relname) {
    IntegerVector ret;
    DBLayer::ResultSet* res;

    string myquery = string("select conkey from pg_constraint join pg_class on pg_class.oid=conrelid where contype='p' and relname='");
    myquery.append( relname->c_str() );
    myquery.append("' ");
    res = this->exec(myquery);

    if( !this->hasErrors() ) {
        string tmp = res->getValue(0,0);
        // Elimino le graffe
        unsigned long apertaGraffa = tmp.find('{');
        if(apertaGraffa!=string::npos) tmp.erase( apertaGraffa, 1 );
        unsigned long chiusaGraffa = tmp.find('}');
        if(chiusaGraffa!=string::npos) tmp.erase( chiusaGraffa, 1 );
        unsigned long br = 0;
        while((br = tmp.find(" ")) != string::npos)
            tmp.erase( br, 1 );
        unsigned long indice = 0;
        int valore=0;
        while( (indice=tmp.find(',') ) != string::npos ) {
            string mysubstr = tmp.substr(0,indice);
            valore = atoi(mysubstr.c_str() );
            ret.push_back( valore );
            tmp = tmp.substr( indice+1 );
        }
        valore = atoi(tmp.c_str() );
        ret.push_back( valore );
    }
    delete res;

    return ret;
}

/*int PGConnection::protocolVersion() {
	return PQprotocolVersion( this->conn );
}
int PGConnection::serverVersion() {
	return PQserverVersion( this->conn );
}*/
string PGConnection::pgtype2string(int t) {
    switch( t ) {
    case 16:
        return DBLayer::type_boolean;
        break;
    case 20:
    case 21:
    case 22:
    case 23:
    case 26:
        return DBLayer::type_integer;
        break;
    case 700:
    case 701:
        return DBLayer::type_double;
        break;
    case 18:
    case 19:
    case 25:
    case 1042:
    case 1043:
        return DBLayer::type_string;
        break;
    case 702:
    case 703:
    case 704:
    case 1082:
    case 1083:
    case 1114:
    case 1184:
    case 1266:
        return DBLayer::type_datetime;
        break;
    default:
        printf("DBLayer::PGConnection::pgtype2string: unknown type = %d\n", t );
        return DBLayer::type_blob;
    }
}
int PGConnection::clientEncoding() { return PQclientEncoding( this->conn ); }
int PGConnection::setClientEncoding(string s) { return PQsetClientEncoding( this->conn, s.c_str() ); }

#endif

//********************* ResultSet
ResultSet::ResultSet() {}
ResultSet::~ResultSet() {}
int ResultSet::getNumColumns() { return (int) this->columnName.size();}
int ResultSet::getNumRows() { return this->columnName.size()!=0 ? (int) ( this->righe.size() / this->columnName.size() ) : 0; }
string ResultSet::getValue(int row, int column) { return this->righe.at( row * this->columnName.size() + column ); }
string ResultSet::getValue(int row, string* columnName) { return this->getValue(row, this->getColumnIndex(columnName)); }
string ResultSet::getColumnName(int i) { return this->columnName[i]; }
string ResultSet::getColumnType(int i) { return this->columnType[i]; }
int ResultSet::getColumnSize(int i) { return this->columnSize[i]; }

int ResultSet::getLength(int row, int column) { return (int) this->righe.at( row * this->columnName.size() + column ).size(); }
bool ResultSet::isNull(int row, int column) {
    string tmp = this->getValue(row,column);
    return tmp=="\\N";
}

string ResultSet::getErrorMessage() { return string(""); }

bool ResultSet::hasErrors() { return this->getErrorMessage().length() > 0; }

string ResultSet::getStatus() { return string( "" ); }

int ResultSet::getColumnIndex(string* columnName ) {
    int ret = -1;
    const char* columnNameChar = columnName->c_str();
    for(unsigned int i=0; i<this->columnName.size() && ret<0; i++) {
        const char* currentColumnNameChar = this->columnName[i].c_str();
        if( strcmp( columnNameChar, currentColumnNameChar )==0 )
            ret = i;
    }
    return ret;
}

string ResultSet::toString(string prefix) {
    string ret;
    ret.append(prefix+"<ResultSet>");

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

    ret.append(prefix+" <Rows>");
    int nRighe = this->getNumRows();
    for(int r=0; r<nRighe; r++) {
        ret.append(prefix+"  <Row num=\'"+DBLayer::integer2string((long)r)+"\'>");
        for(int c=0; c<nColonne; c++) {
            string nomeColonna = this->getColumnName(c);
            if (! this->isNull(r,c) ) {
                ret.append(prefix+"   <"+nomeColonna+" "+
                           "size=\'" +	DBLayer::integer2string((long)this->getColumnSize(c)) + "\' " +
                           "length=\'" +	DBLayer::integer2string((long)this->getLength(r,c)) + "\' " +
                           ">");
                ret.append( this->getValue(r,c) );
                ret.append("</"+nomeColonna+">");
            } else {
                ret.append(prefix+"   <"+nomeColonna+" />");
            }
        }
        ret.append(prefix+"  </Row>");
    }
    ret.append(prefix+" </Rows>");

    ret.append(prefix+"</ResultSet>");
    return ret;
}

//********************* PGResultSet

#ifdef USE_LIBPQ

PGResultSet::PGResultSet() : ResultSet::ResultSet() {}
PGResultSet::PGResultSet(PGresult* res) : ResultSet::ResultSet() { this->res = res; }
PGResultSet::~PGResultSet() { PQclear(this->res); }
int PGResultSet::getNumColumns() { return PQnfields( this->res ); }
int PGResultSet::getNumRows() { return PQntuples( this->res ); }
string PGResultSet::getValue(int row, int column) {
    string ret;
    char* tmp = PQgetvalue( this->res, row, column );
    if(tmp!=0) ret.append(tmp);
    return ret;
}
string PGResultSet::getColumnName(int i) { return string( PQfname(this->res, i) ); }
/*	I tipi di postgres si vedono con la query: select oid,typname from pg_type	*/
string PGResultSet::getColumnType(int i) {
    int mytype = (int) PQftype( this->res, i );
    return PGConnection::pgtype2string(mytype);
//    switch( mytype ) {
//    case 16:
//        return DBLayer::type_boolean;
//        break;
//    case 20:
//    case 21:
//    case 22:
//    case 23:
//    case 26:
//        return DBLayer::type_integer;
//        break;
//    case 700:
//    case 701:
//        return DBLayer::type_double;
//        break;
//    case 18:
//    case 19:
//    case 25:
//    case 1042:
//    case 1043:
//        return DBLayer::type_string;
//        break;
//    case 702:
//    case 703:
//    case 704:
//    case 1082:
//    case 1083:
//    case 1114:
//    case 1184:
//    case 1266:
//        return DBLayer::type_datetime;
//        break;
//    default:
//        printf("DBLayer::PGResultSet::getColumnType: unknown type = %d\n", mytype );
//        return DBLayer::type_blob;
//    }
}

int PGResultSet::getColumnSize(int i) { return PQfsize(this->res, i); }
int PGResultSet::getLength(int row, int column) { return PQgetlength(this->res, row, column); }
bool PGResultSet::isNull(int row, int column) { return PQgetisnull(this->res, row, column)>0 ? true : false; }
string PGResultSet::getErrorMessage() { return string( PQresultErrorMessage(this->res) ); }
string PGResultSet::getStatus() { return string( PQresStatus( PQresultStatus( (this->res) ) ) ); }
int PGResultSet::getColumnIndex(string* columnName ) { return PQfnumber( this->res, columnName->c_str() ); }

#endif
