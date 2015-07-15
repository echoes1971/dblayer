/***************************************************************************
**	dbconnection.cpp  v0.1.2 - 2012.03.19
**	-----------------------------------
**
**	Author:		Roberto Rocco Angeloni.
**	E-mail:		roberto@roccoangeloni.it
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
** @copyright &copy; 2011-2015 by Roberto Rocco Angeloni <roberto@roccoangeloni.it>
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
#include "config.h"
#endif
#ifdef USE_LIBPQ
 #include "dbconnection.h"
#endif
#include "mysqlconnection.h"
#include "odbcconnection.h"
#include "sqliteconnection.h"
#include "xmlrpcconnection.h"
using namespace DBLayer;

ConnectionBuildersMap DBLayer::connectionBuilders;

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
    this->verbose = false;

    this->_logger =  [] (const string& s, const bool newline) -> void {
        cout << s;
        if(newline) cout << endl;
    };
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

bool Connection::isConnected() const { return this->connected; }

ResultSet* Connection::login(string user, string pwd) {
    string query("");
    query.append("select * from auth_users where login='").append(this->escapeString(user))
        .append("' and pwd='").append(this->escapeString(pwd)).append("'");
    return this->exec(query);
    //return new ResultSet();
}

string Connection::getFormSchema(string language) { return "Connection::getFormSchema: NOT SUPPORTED - " + language; }
string Connection::getDBSchema(string language) { return "Connection::getDBSchema: NOT SUPPORTED - " + language; }
string Connection::getSchemaName() const { return "Connection::getSchemaName: NOT SUPPORTED"; }
string Connection::getDBType() const { return "generic"; }

string Connection::dbeType2dbType(const string& dbetype) const {
    string ret = dbetype;
//    if(dbetype=="int")
//        ret = "int(11)";
//    else
        if(dbetype=="uuid")
            ret = "char(40)";
    return ret;
}
string Connection::dbType2dbeType(const string& dbtype) const {
    string ret = dbtype;
    if(dbtype=="int(11)")
        ret = "int";
    else if(dbtype=="char(40)")
        ret = "uuid";
    return ret;
}

string Connection::dbConstraints2dbeConstraints(map<string,string>& def) const {
    StringVector constraints;
    if( def["Null"]=="NO") constraints.push_back("not null");
    if( def.find("Default")!=def.end() ) {
        string apice ="\'";
        if( def["Type"]=="int(11)" || def["Type"]=="float")
            apice = "";
        constraints.push_back( string("default ").append(apice).append(def["Default"]).append(apice) );
    } else if(def.find("Default")==def.end() && def["Null"]=="YES") {
        constraints.push_back("default null");
    }
    string glue(" ");
    return constraints.size()>0 ? DBLayer::joinString(&constraints, &glue) : "";
}
string Connection::dbColumnDefinition2dbeColumnDefinition(map<string,string>& def) const {
    string constraints = Connection::dbConstraints2dbeConstraints(def);
    return string("'$col'=>array('").append( Connection::dbType2dbeType(def["Type"]) ).append("',")
            .append( constraints.length()>0 ? constraints : "" )
            .append("),\n");
}

ResultSet* Connection::exec(const string s) { return new ResultSet(); }

string Connection::getErrorMessage() const { return this->errorMessage; }
bool Connection::hasErrors() const { return this->getErrorMessage().length() > 0; }

void Connection::setVerbose(bool b) { this->verbose=b; }
bool Connection::isVerbose() const { return this->verbose; }

void Connection::setLogger(std::function<void(const string&,bool)> logger) { this->_logger = logger; }
void Connection::log(const string& s, const bool newline) const { this->_logger(s,newline); }

bool Connection::flush() { return true; }

bool Connection::reconnect() { return false; }

string* Connection::escapeString(string* s) const { return s; }
string Connection::escapeString(string s) const { return DBLayer::replaceAll(s, "\'", "\'\'"); }
string Connection::quoteDate(string s) const { return "'"+DBLayer::replaceAll(s, " 00:00:00", "")+"'"; }

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
bool Connection::isProxy() const { return false; }
DBEntity* Connection::Insert(DBEntity *dbe) { return dbe; }
DBEntity* Connection::Update(DBEntity *dbe) { return dbe; }
DBEntity* Connection::Delete(DBEntity *dbe) { return dbe; }
DBEntityVector* Connection::Select(const DBEntity *dbe, const string &tableName, const string& searchString) { return (DBEntityVector*) (0 & (long)dbe & (long)tableName.size() & (long)searchString.size()); }
DBEntityVector* Connection::Search(DBEntity* dbe, bool uselike, bool caseSensitive, const string& orderBy ) { return (DBEntityVector*) (0 & (long)dbe & (long)uselike & (long)caseSensitive & (long)orderBy.size()); }
string Connection::ping() { return "pong"; }

void Connection::setDBEFactory(DBEFactory* factory) { this->factory = factory; }
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
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        this->errorMessage = string( PQresultErrorMessage(res) );
        //PQclear(res);	caller MUST destroy the PGResultSet
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
                row.push_back( res->getValue(r,c++) );
            else
                row.push_back( "\\N" );
            ret[ res->getValue(r,0) ] = row;
        }
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
        // Removing {} parenthesis
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

string PGConnection::dbeType2dbType(const string& dbetype) const {
    string ret = dbetype;
    if(dbetype=="datetime") {
        ret = "timestamp";
    } else if(dbetype=="uuid")
        ret = "char(40)";
    return ret;
}

#endif

//********************* ResultSet
ResultSet::ResultSet() {}
ResultSet::~ResultSet() {}
int ResultSet::getNumColumns() const { return (int) this->columnName.size();}
int ResultSet::getNumRows() const { return this->columnName.size()!=0 ? (int) ( this->righe.size() / this->columnName.size() ) : 0; }
string ResultSet::getValue(int row, int column) const { return this->righe.at( row * this->columnName.size() + column ); }
string ResultSet::getValue(int row, string* columnName) const { return this->getValue(row, this->getColumnIndex(columnName)); }
string ResultSet::getColumnName(int i) const { return this->columnName[i]; }
string ResultSet::getColumnType(int i) const { return this->columnType[i]; }
int ResultSet::getColumnSize(int i) const { return ((long)this->columnSize.size())>=(((long)i)+1) ? this->columnSize[i] : -1; }

int ResultSet::getLength(int row, int column) const { return (int) this->righe.at( row * this->columnName.size() + column ).size(); }
bool ResultSet::isNull(int row, int column) const {
    string tmp = this->getValue(row,column);
    return tmp=="\\N";
}

string ResultSet::getErrorMessage() const { return string(""); }

bool ResultSet::hasErrors() const { return this->getErrorMessage().length() > 0; }

string ResultSet::getStatus() const { return string( "" ); }

int ResultSet::getColumnIndex(string* columnName ) const {
    int ret = -1;
    const char* columnNameChar = columnName->c_str();
    for(unsigned int i=0; i<this->columnName.size() && ret<0; i++) {
        const char* currentColumnNameChar = this->columnName[i].c_str();
        if( strcmp( columnNameChar, currentColumnNameChar )==0 )
            ret = i;
    }
    return ret;
}

string ResultSet::toString(string prefix) const {
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
int PGResultSet::getNumColumns() const { return PQnfields( this->res ); }
int PGResultSet::getNumRows() const { return PQntuples( this->res ); }
string PGResultSet::getValue(int row, int column) const {
    string ret;
    char* tmp = PQgetvalue( this->res, row, column );
    if(tmp!=0) ret.append(tmp);
    return ret;
}
string PGResultSet::getColumnName(int i) const { return string( PQfname(this->res, i) ); }
/*	I tipi di postgres si vedono con la query: select oid,typname from pg_type	*/
string PGResultSet::getColumnType(int i) const {
    int mytype = (int) PQftype( this->res, i );
    return PGConnection::pgtype2string(mytype);
}

int PGResultSet::getColumnSize(int i) const { return PQfsize(this->res, i); }
int PGResultSet::getLength(int row, int column) const { return PQgetlength(this->res, row, column); }
bool PGResultSet::isNull(int row, int column) const { return PQgetisnull(this->res, row, column)>0 ? true : false; }
string PGResultSet::getErrorMessage() const { return string( PQresultErrorMessage(this->res) ); }
string PGResultSet::getStatus() const { return string( PQresStatus( PQresultStatus( (this->res) ) ) ); }
int PGResultSet::getColumnIndex(string* columnName ) const { return PQfnumber( this->res, columnName->c_str() ); }

#endif
