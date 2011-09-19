/***************************************************************************
**	MySQLConnection.cpp  v0.1.1 - 2006.05.14
**	-----------------------------------------
**
**	Author:		Roberto Rocco Angeloni.
**	email:		roberto@roccoangeloni.it
**	Comment:	Implementazione di connection e resultset per SQLite
**	To Do:		- SQLiteResultSet::getColumnType: RICONOSCERE SE E' UNA DATA!!!
**	Future:
**	History:
**		v0.1.0 - 2006.05.11 Iniziato lo sviluppo
**		v0.1.1 - 2006.05.14 Completato SQLiteResultSet e MySQLConnection ad
**				 eccezione di alcuni metodi che non sono supportati in sqlite3
**
** @copyright &copy; 2011 by Roberto Rocco Angeloni <roberto@roccoangeloni.it>
** @license http://opensource.org/licenses/lgpl-3.0.html GNU Lesser General Public License, version 3.0 (LGPLv3)
** @version $Id: mysqlconnection.cpp $
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

#include <stdio.h>

#include "mysqlconnection.h"
using namespace DBLayer;

#ifdef USE_MYSQL


//********************* MySQLConnection: inizio.
#ifdef WIN32
MySQLConnection::MySQLConnection(string s) : Connection(s) {
#else
MySQLConnection::MySQLConnection(string s) : Connection::Connection(s) {
// MySQLConnection::MySQLConnection(string s) : Connection(s) {
#endif
    this->db = 0;

// 	cout << "MySQLConnection::MySQLConnection: s=" << s << endl;
    // Parsing connection string
#if __i386__
    unsigned int indiceInizioStringa = s.find("host=", 0) + 5;
    unsigned int indiceFineStringa = s.find(';', indiceInizioStringa);
#else
    unsigned long indiceInizioStringa = s.find("host=", 0) + 5;
    unsigned long indiceFineStringa = s.find(';', indiceInizioStringa);
#endif
    if( indiceInizioStringa != string::npos )
        this->host = s.substr(indiceInizioStringa, indiceFineStringa-indiceInizioStringa );
    //cout << "MySQLConnection::MySQLConnection: inizio="<<indiceInizioStringa<< " fine="<<indiceFineStringa<<" host=" << this->host << endl;
    indiceInizioStringa = s.find("dbname=", 0) + 7;
    indiceFineStringa = s.find(";", indiceInizioStringa);
    if( indiceInizioStringa != string::npos )
        this->dbname = s.substr(indiceInizioStringa, indiceFineStringa-indiceInizioStringa );
    //cout << "MySQLConnection::MySQLConnection: inizio="<<indiceInizioStringa<< " fine="<<indiceFineStringa<<" dbname=" << this->dbname << endl;
    indiceInizioStringa = s.find("user=", 0) + 5;
    indiceFineStringa = s.find(";", indiceInizioStringa);
    if( indiceInizioStringa != string::npos )
        this->user = s.substr(indiceInizioStringa, indiceFineStringa-indiceInizioStringa );
    //cout << "MySQLConnection::MySQLConnection: inizio="<<indiceInizioStringa<< " fine="<<indiceFineStringa<<" user=" << this->user << endl;
    indiceInizioStringa = s.find("password=", 0) + 9;
    indiceFineStringa = s.find(";", indiceInizioStringa);
    if( indiceInizioStringa != string::npos )
        this->password = s.substr(indiceInizioStringa, indiceFineStringa-indiceInizioStringa );
    //cout << "MySQLConnection::MySQLConnection: inizio="<<indiceInizioStringa<< " fine="<<indiceFineStringa<<" password=" << this->password << endl;
}
MySQLConnection::~MySQLConnection() {
//	cout << "MySQLConnection::~MySQLConnection: inizio." << endl;
    this->disconnect();
//	cout << "MySQLConnection::~MySQLConnection: fine." << endl;
}

bool MySQLConnection::connect() {
    if(this->db!=0)
        this->disconnect();
        //return true;
    this->db = mysql_init(0); //this->db);
    this->errorMessage.clear();
    if(!this->db) {
        this->errorMessage.append( "MySQLConnection::connect: Unable to connect to "+this->dbname+"@"+this->host+" with user "+this->user+" ("+DBLayer::integer2string((long)this->db)+") "+ "!" );
        return false;
    }
//    MYSQL* tmp = mysql_real_connect(
    this->db = mysql_real_connect(
                this->db,
                this->host.c_str(),
                this->user.c_str(),
                this->password.c_str(),
                this->dbname.c_str(),
                0, 0, 0);
    unsigned int errorCode = mysql_errno(this->db);
    if( errorCode!=0 ) {
        cout << "MySQLConnection::connect: errorCode = " << errorCode << endl;
        this->errorMessage.append(
                    "errorCode: "+ DBLayer::integer2string((long)errorCode)
                    + "; msg: " + mysql_error(this->db)
                    );
        cout << "MySQLConnection::connect: errorMessage = " << this->errorMessage << endl;
        return false;
    }
    this->connected = true;
    return true;
}
bool MySQLConnection::disconnect() {
// 	cout << "MySQLConnection::disconnect: inizio." << endl;
// 	cout << "MySQLConnection::disconnect: this->db=" + DBLayer::integer2string((long)this->db)<<endl;
    unsigned int errorCode;
    if( this->db!=0 ) {
        errorCode = mysql_errno(this->db);
        if( errorCode!=0 ) {
            // 			cout << "MySQLConnection::disconnect: errorCode = " << errorCode << endl;
            this->errorMessage.append(
                        "errorCode: "+ DBLayer::integer2string((long)errorCode)
                        + "; msg: " + mysql_error(this->db)
                        );
            // 			cout << "MySQLConnection::disconnect: errorMessage = " << this->errorMessage << endl;
            return false;
        }
        mysql_close(this->db);
        this->db = 0;
    }
    // 	cout << "MySQLConnection::disconnect: fine." << endl;
    this->connected = false;
    return true;
}
bool MySQLConnection::reconnect() {
    if( this->disconnect() )
        return this->connect();
    else
        return false;
}

bool myDebugExec = false;


ResultSet* MySQLConnection::exec(const string s) {
    int errorCode;
    const char* query = s.c_str();
    MySQLResultSet* rs = new MySQLResultSet();
//    if(myDebugExec) cout << "MySQLConnection::exec: query = " << query << endl;

    this->errorMessage.clear();

    errorCode = mysql_query(this->db, query);

    if( errorCode!=0 ) {
        this->errorMessage.append(
                    "errorCode: "+ DBLayer::integer2string((long)errorCode)
                    + "; msg: " + mysql_error(this->db)
                    );
        return rs;
    }
    MYSQL_RES *result = mysql_store_result(this->db);
    if(result==0) {
//        if(myDebugExec)	cout << "MySQLConnection::exec: result==NULL " << endl;
        return rs;
    }

    // Preparo i metadati
//    if(myDebugExec)	cout << "MySQLConnection::exec: Preparo i metadati " << endl;
//    unsigned int nColonne = mysql_num_fields(result);
//    if(myDebugExec)	cout << "MySQLConnection::exec: nColonne=" << DBLayer::integer2string((long)nColonne) << endl;

    MYSQL_FIELD *field;
    while((field = mysql_fetch_field(result))) {
        // 	for( unsigned int i=0; i<nColonne; i++) {
        const char* nomeColonna = field->name;
        if(myDebugExec)	cout << "MySQLConnection::exec: nomeColonna = " << nomeColonna << "; type = " << DBLayer::integer2string((long)field->type) << "; length = " << DBLayer::integer2string((long)field->length) << endl;
        rs->columnName.push_back( string(nomeColonna) );
        rs->columnSize.push_back( (int) field->length );
        //rs->columnSize.push_back( field->length>0 ? field->length : field->max_length );

        //delete nomeColonna;
        string nomeTipo = MySQLConnection::getNomeTipo(field);
        if(myDebugExec)	cout << "MySQLConnection::exec: nomeTipo=\'" << nomeTipo << "\'" << endl;
        if(nomeTipo.length()>0) {
            if(nomeTipo == "integer") {
                rs->columnType.push_back( DBLayer::type_integer );
            } else if( nomeTipo == "string" ) {
                rs->columnType.push_back( DBLayer::type_string );
            } else if( nomeTipo == "float" ) {
                rs->columnType.push_back( DBLayer::type_double );
            } else if( nomeTipo == "datetime" ) {
                rs->columnType.push_back( DBLayer::type_datetime );
            } else if( nomeTipo == "blob" ) {
                rs->columnType.push_back( DBLayer::type_blob );
            } else {
                if(myDebugExec) cout << "MySQLConnection::exec: codiceTipo \'" << nomeTipo
                                     << "\' NON riconosciuto!" << endl;
                rs->columnType.push_back( DBLayer::type_blob );
            }
        } else {
            if(myDebugExec)	cout << "MySQLConnection::exec: nomeTipo VUOTO" << endl;
            rs->columnType.push_back( DBLayer::type_blob );
        }
        // delete nomeTipo;
    }

    MYSQL_ROW row;
    if(myDebugExec)	cout << "MySQLConnection::exec: Leggo tutte le righe " << endl;
    my_ulonglong num_rows = mysql_num_rows(result);
    if(myDebugExec)	cout << "MySQLConnection::exec: num_rows=\'" << DBLayer::integer2string((long)num_rows) << "\'" << endl;
    while ((row = mysql_fetch_row(result))) {
        unsigned long *lengths;
        lengths = mysql_fetch_lengths(result);
        for(unsigned int c=0; c<rs->columnName.size(); c++) {
            //int sizeDato = lengths[c];

            //if(myDebugExec)	cout << "MySQLConnection::exec: sizeDato=\'" << DBLayer::integer2string((long)sizeDato) << "\'" << endl;
            //if(sizeDato>0 && row[c]) {
            //if(myDebugExec)	cout << "MySQLConnection::exec: row[c]=\'" << DBLayer::integer2string((long)row[c]) << "\'" << endl;
            if(row[c]!=0) {
                //if(myDebugExec)	printf("[%.*s] ", (int) lengths[c], row[c] ? row[c] : "NULL");
                char* tmpValore = new char[ lengths[c] + 2 ];
                char* tmpSorgente = row[c];
                snprintf( tmpValore, lengths[c] + 2, "%s", tmpSorgente );
                //if(myDebugExec)	cout << "MySQLConnection::exec: tmpValore=\'" << tmpValore << "\'" << endl;
                // 2011.09.04: start.
                string valore(tmpValore);
                //string valore = string( tmpValore );
                // 2011.09.04: start.
                rs->righe.push_back( valore );
                delete[] tmpValore;
            } else if (row[c]==0 && ( rs->columnType[c]==DBLayer::type_integer || rs->columnType[c]==DBLayer::type_double) ) {
                //if(myDebugExec)	printf("[%.*s] ", (int) lengths[c], row[c] ? row[c] : "NULL");
                char* tmpValore = new char[ lengths[c] + 2 ];
                char* tmpSorgente = row[c];
#ifdef __i386__
                sprintf( tmpValore, "%d", (int) tmpSorgente );
#else
                snprintf( tmpValore, lengths[c] + 2, "%ld", (unsigned long) tmpSorgente );
#endif
                //if(myDebugExec)	cout << "MySQLConnection::exec: tmpValore=\'" << tmpValore << "\'" << endl;
                string valore( tmpValore );
                rs->righe.push_back( valore );
                delete[] tmpValore;
            } else {
                rs->righe.push_back( string("\\N") );
            }
        }
        //if(myDebugExec)	printf("\n");
    }
    mysql_free_result(result);

    /*	errorCode = mysql_query(this->db, query);
 if( errorCode!=0 ) {
  this->errorMessage.append(
   "errorCode: "+ DBLayer::integer2string((long)errorCode)
   + "; msg: " + mysql_error(this->db)
  );
  return rs;
 }
*/

    //	if(myDebugExec)	cout << "MySQLConnection::exec: rs=" << rs->toString() << endl;
    //	if(myDebugExec)	cout << "MySQLConnection::exec: fine." << endl;
    return rs;
}

string MySQLConnection::escapeString(string s) {
    static string fromQuote("\'");
    static string   toQuote("\'\'");
    return DBLayer::replaceAll(s, fromQuote, toQuote);
}
string MySQLConnection::getNomeTipo(st_mysql_field* field) {
    switch( (long) field->type ) {
    case MYSQL_TYPE_VARCHAR:
    case MYSQL_TYPE_VAR_STRING:
    case MYSQL_TYPE_STRING:
        return string("string");
    case MYSQL_TYPE_TINY:
    case MYSQL_TYPE_SHORT:
    case MYSQL_TYPE_LONG:
    case MYSQL_TYPE_LONGLONG:
    case MYSQL_TYPE_INT24:
    case MYSQL_TYPE_BIT:
        return string("integer");
    case MYSQL_TYPE_NEWDECIMAL:
    case MYSQL_TYPE_DECIMAL:
    case MYSQL_TYPE_FLOAT:
    case MYSQL_TYPE_DOUBLE:
        return ("float");
    case MYSQL_TYPE_TIMESTAMP:
    case MYSQL_TYPE_DATE:
    case MYSQL_TYPE_TIME:
    case MYSQL_TYPE_DATETIME:
    case MYSQL_TYPE_YEAR:
    case MYSQL_TYPE_NEWDATE:
        return string("datetime");
    case MYSQL_TYPE_TINY_BLOB:
    case MYSQL_TYPE_MEDIUM_BLOB:
    case MYSQL_TYPE_LONG_BLOB:
    case MYSQL_TYPE_BLOB:
        return string("blob");

    case MYSQL_TYPE_NULL:
    case MYSQL_TYPE_ENUM:
    case MYSQL_TYPE_SET:
    case MYSQL_TYPE_GEOMETRY:
        return string("unknown");
    }
    return string("unknown");
}

int MySQLConnection::getColumnSize(string* relname) {
    MYSQL_RES *result = mysql_list_fields(this->db, relname->c_str(), 0);

    if(result==0) {
        return -1;
    }
    unsigned int lengths;
    lengths = mysql_field_count(this->db);
    mysql_free_result(result);

    return (int) lengths;
}
string MySQLConnection::getColumnName(string* relname, int column) {
    string ret("");
    MYSQL_RES *result = mysql_list_fields(this->db, relname->c_str(), 0);

    if(result==0) {
        //cout << "MySQLConnection::getColumnName: result==NULL " << endl;
        return "";
    }
    unsigned int lengths;
    lengths = mysql_field_count(this->db);
    //cout << "MySQLConnection::getColumnName: lengths=\'" << DBLayer::integer2string((long)lengths) << "\'" << endl;
    MYSQL_FIELD *field;
    for(unsigned int i=0; ret=="" && i<lengths; i++) {
        field = mysql_fetch_field(result);
        const char* nomeColonna = field->name;
        if( ((int) i+1) == column ) {
            ret.append( nomeColonna );
        }
        //cout << "MySQLConnection::getColumnName: field=\'" << nomeColonna << " chiave=" << DBLayer::integer2string((long)field->flags&PRI_KEY_FLAG) << " chiave multipla=" << DBLayer::integer2string((long)field->flags&MULTIPLE_KEY_FLAG) << "\'" << endl;
    }
    mysql_free_result(result);

    return ret;
}
IntegerVector MySQLConnection::getKeys(string* relname) {
    MYSQL_RES *result = mysql_list_fields(this->db, relname->c_str(), 0);
    IntegerVector ret;

    if(result==0) {
        //cout << "MySQLConnection::getKeys: result==NULL " << endl;
        return ret;
    }
    unsigned int lengths;
    lengths = mysql_field_count(this->db);
    //cout << "MySQLConnection::getKeys: lengths=\'" << DBLayer::integer2string((long)lengths) << "\'" << endl;
    MYSQL_FIELD *field;
    for(unsigned int i=0; ret.size()==0 && i<lengths; i++) {
        field = mysql_fetch_field(result);
        if( (field->flags & PRI_KEY_FLAG) > 0 ) {
            ret.push_back( (int) i+1 );
        }
        //const char* nomeColonna = field->name;
        //cout << "MySQLConnection::getKeys: field=\'" << nomeColonna << " chiave=" << DBLayer::integer2string((long)field->flags&PRI_KEY_FLAG) << " chiave multipla=" << DBLayer::integer2string((long)field->flags&MULTIPLE_KEY_FLAG)<< "\'" << endl;
    }
    mysql_free_result(result);
    return ret;
}
IntegerVector MySQLConnection::getForeignKeys(string* relname) {
    MYSQL_RES *result = mysql_list_fields(this->db, relname->c_str(), 0);
    IntegerVector ret;

    if(result==0) {
        //cout << "MySQLConnection::getKeys: result==NULL " << endl;
        return ret;
    }
    unsigned int lengths;
    lengths = mysql_field_count(this->db);
    //cout << "MySQLConnection::getKeys: lengths=\'" << DBLayer::integer2string((long)lengths) << "\'" << endl;
    MYSQL_FIELD *field;
    for(unsigned int i=0; ret.size()==0 && i<lengths; i++) {
        field = mysql_fetch_field(result);
        this->printField(field);
        if( ( (field->flags&PRI_KEY_FLAG) == 0 ) && ( (field->flags&MULTIPLE_KEY_FLAG) == MULTIPLE_KEY_FLAG ) ) {
            ret.push_back( (int) i+1 );
        }
        //const char* nomeColonna = field->name;
        //cout << "MySQLConnection::getKeys: field=\'" << nomeColonna << " chiave=" << DBLayer::integer2string((long)field->flags&PRI_KEY_FLAG) << " chiave multipla=" << DBLayer::integer2string((long)field->flags&MULTIPLE_KEY_FLAG)<< "\'" << endl;
    }
    mysql_free_result(result);
    return ret;
}

void MySQLConnection::printField(MYSQL_FIELD* field) {
    cout << "MySQLConnection::printField: field->name=\'" << field->name <<  "\'" << endl;
    cout << "MySQLConnection::printField: field->org_name=\'" << field->org_name <<  "\'" << endl;
    cout << "MySQLConnection::printField: field->table=\'" << field->table <<  "\'" << endl;
    cout << "MySQLConnection::printField: field->org_table=\'" << field->org_table <<  "\'" << endl;
    cout << "MySQLConnection::printField: field->db=\'" << field->db <<  "\'" << endl;
    cout << "MySQLConnection::printField: field->catalog=\'" << field->catalog <<  "\'" << endl;
// 		cout << "MySQLConnection::printField: field->def=\'" << field->def <<  "\'" << endl;
}

//********************* MySQLConnection: fine.



//********************* MySQLResultSet: inizio.

#ifdef WIN32
MySQLResultSet::MySQLResultSet() : ResultSet() {
#else
MySQLResultSet::MySQLResultSet() : ResultSet::ResultSet() {
#endif
}


MySQLResultSet::~MySQLResultSet() {
//	cout << "SQLiteResultSet::~SQLiteResultSet: inizio." << endl;
//	cout << "SQLiteResultSet::~SQLiteResultSet: fine." << endl;
}

string MySQLResultSet::toString(string prefix) {
    string ret;
    ret.append(prefix+"<MySQLResultSet>");

//	cout << "SQLiteResultSet::toString: Stampo le colonne." << endl;
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

//	cout << "SQLiteResultSet::toString: Stampo le righe." << endl;
    ret.append(prefix+" <Rows>");
    int nRighe = this->getNumRows();
    for(int r=0; r<nRighe; r++) {
//		cout << "SQLiteResultSet::toString: riga = " << DBLayer::integer2string((long)r) << endl;
        ret.append(prefix+"  <Row num=\'"+DBLayer::integer2string((long)r)+"\'>");
        for(int c=0; c<nColonne; c++) {
            string nomeColonna = this->getColumnName(c);
//			cout << "SQLiteResultSet::toString: nomeColonna = " << nomeColonna << endl;
//			cout << "SQLiteResultSet::toString: valoreColonna = " << this->getValue(r,c) << endl;
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

    ret.append(prefix+"</MySQLResultSet>");
    return ret;
}
//********************* MySQLResultSet: fine.

#endif
