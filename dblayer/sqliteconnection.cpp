/***************************************************************************
**	sqliteconnection.cpp  v0.1.1 - 2006.05.14
**	-----------------------------------------
**
**	Author:		Roberto Rocco Angeloni.
**	email:		roberto@roccoangeloni.it
**	Comment:	Implementazione di connection e resultset per SQLite
**	To Do:		- SQLiteResultSet::getColumnType: RICONOSCERE SE E' UNA DATA!!!
**	Future:
**	History:
**		v0.1.0 - 2006.05.11 Iniziato lo sviluppo
**		v0.1.1 - 2006.05.14 Completato SQLiteResultSet e SQLiteConnection ad
**				 eccezione di alcuni metodi che non sono supportati in sqlite3
**
** @copyright &copy; 2011 by Roberto Rocco Angeloni <roberto@roccoangeloni.it>
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
#include <config.h>

#include <cstdio>

#include "sqliteconnection.h"
using namespace DBLayer;

#ifdef USE_LIBSQLITE3


//********************* SQLiteConnection: inizio.

#ifdef WIN32
SQLiteConnection::SQLiteConnection(string s) : Connection(s) {
#else
SQLiteConnection::SQLiteConnection(string s) : Connection::Connection(s) {
#endif
	this->db = 0;
}
SQLiteConnection::~SQLiteConnection() {
//	cout << "SQLiteConnection::~SQLiteConnection: inizio." << endl;
	this->disconnect();
//	cout << "SQLiteConnection::~SQLiteConnection: fine." << endl;
}

bool SQLiteConnection::connect() {
	int rc;
        const char* tmp = this->connectionString.c_str();
        rc = sqlite3_open( tmp , &this->db);
	this->errorMessage.clear();
	if(rc) {
		sqlite3_close(this->db);
		this->errorMessage.append( sqlite3_errmsg(this->db) );
		return false;
	}
	return true;
}
bool SQLiteConnection::disconnect() {
//	cout << "SQLiteConnection::disconnect: inizio." << endl;
	int errorCode;
	if( this->db!=0 ) {

		errorCode = sqlite3_close(this->db);

		if( errorCode!=SQLITE_OK ) {
			cout << "SQLiteConnection::disconnect: errorCode = " << errorCode << endl;
			this->errorMessage.append(
				"errorCode: "+ DBLayer::integer2string((long)errorCode) +
				"; msg: " + sqlite3_errmsg(this->db)
			);
			cout << "SQLiteConnection::disconnect: errorMessage = " << this->errorMessage << endl;
			return false;
		}

		this->db = 0;
	}
//	cout << "SQLiteConnection::disconnect: fine." << endl;
	return true;
}
bool SQLiteConnection::reconnect() {
	if( this->disconnect() )
		return this->connect();
	else
		return false;
}
int SQLiteConnection::sqliteCallback(void *NotUsed, int argc, char **argv, char **azColName) {
  int i;
  SQLiteResultSet* rs = (SQLiteResultSet*) NotUsed;

//  rs->columnName.clear();
  for(i=0; i<argc; i++){
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
//    rs->columnName.push_back( azColName[i] );

  }
  printf("\n");

//  if(rs->nColonne < argc) rs->nColonne = argc;
  cout << "SQLiteConnection::sqliteCallback: rs = " << rs << endl;
  return 0;
}

bool debugExec = false;

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

    if(debugExec) cout << "SQLiteConnection::exec: query = " << query << endl;

    //cout << "SQLiteConnection::exec: rs = " << rs << endl;
    this->errorMessage.clear();

    errorCode = sqlite3_prepare(
                this->db,               /* Database handle */
                query,       /* SQL statement, UTF-8 encoded */
                s.size(),             /* Length of zSql in bytes. */
                &pStmt,  /* OUT: Statement handle */
                &zTail     /* OUT: Pointer to unused portion of zSql */
                );

    if( errorCode!=SQLITE_OK ) {
        cout << "SQLiteConnection::exec: errorCode = " << errorCode << endl;
        this->errorMessage.append(
                    "errorCode: "+ DBLayer::integer2string((long)errorCode) +
                    "; msg: " + sqlite3_errmsg(this->db)
                    );
        cout << "SQLiteConnection::exec: errorMessage = " << this->errorMessage << endl;
        cout << "SQLiteConnection::exec: error query = " << query << endl;
        return rs;
    }

    // Preparo i metadati
    if(debugExec) cout << "SQLiteConnection::exec: Preparo i metadati " << endl;
    int nColonne = sqlite3_column_count(pStmt);
    if(debugExec) cout << "SQLiteConnection::exec: nColonne=" << DBLayer::integer2string((long)nColonne) << endl;
    for( int i=0; i<nColonne; i++) {
        const char* nomeColonna = sqlite3_column_name(pStmt,i);
        //cout << "SQLiteConnection::exec: nomeColonna = " << nomeColonna << endl;
        rs->columnName.push_back( string(nomeColonna) );
        //delete nomeColonna;
        const char* nomeTipo = sqlite3_column_decltype(pStmt,i);
        if(nomeTipo!=0) {
            //if(debugExec) cout << "SQLiteConnection::exec: nomeTipo=\'" << nomeTipo << "\'" << endl;
            if( strcmp(nomeTipo,"int")==0 || strcmp(nomeTipo,"integer")==0) {
                rs->columnType.push_back( DBLayer::type_integer );
            } else if( strcmp(nomeTipo,"text"  )==0 ) {
                rs->columnType.push_back( DBLayer::type_string );
            } else if( strcmp(nomeTipo,"float"  )==0 ) {
                rs->columnType.push_back( DBLayer::type_double );
            } else if( strcmp(nomeTipo,"blob"  )==0 ) {
                rs->columnType.push_back( DBLayer::type_blob );
            } else {
                cout << "SQLiteConnection::exec: codiceTipo \'" << nomeTipo
                     << "\' NON riconosciuto!" << endl;
                rs->columnType.push_back( DBLayer::type_blob );
            }
        } else {
            //cout << "SQLiteConnection::exec: nomeTipo VUOTO" << endl;
            rs->columnType.push_back( DBLayer::type_blob );
        }
        //		delete nomeTipo;
    }

    // Leggo la prima riga
    errorCode = sqlite3_step(pStmt);
    if(debugExec) cout << "SQLiteConnection::exec: Leggo la prima riga " << endl;
    if( errorCode!=SQLITE_ROW && errorCode!=SQLITE_DONE ) {
        cout << "SQLiteConnection::exec: errorCode = " << errorCode << endl;
        this->errorMessage.append(
                    "errorCode: "+ DBLayer::integer2string((long)errorCode) +
                    "; msg: " + sqlite3_errmsg(this->db)
                    );
        cout << "SQLiteConnection::exec: errorMessage = " << this->errorMessage << endl;
    }

    // Leggo tutte le righe
    if(debugExec)	cout << "SQLiteConnection::exec: Leggo tutte le righe " << endl;
    while( errorCode==SQLITE_ROW ) {
        for(unsigned int c=0; c<rs->columnName.size(); c++) {
            int sizeDato = sqlite3_column_bytes(pStmt,c);
            // SE il tipo e' blob, allora probabilmente non e' stato identificato
            // inizialmente, quindi lo aggiorniamo mentre leggiamo la riga
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
                    //					case SQLITE3_TEXT:
                    rs->columnType[c]=DBLayer::type_string;
                    break;
                }
            }

            if(debugExec)	cout << "SQLiteConnection::exec: sizeDato=\'" << DBLayer::integer2string((long)sizeDato) << "\'" << endl;
            if(sizeDato>0) {
                char* tmpValore = new char[ sqlite3_column_bytes(pStmt,c) * 2 ];
                const unsigned char* tmpSorgente = sqlite3_column_text(pStmt,c);
                sprintf( tmpValore, "%s", tmpSorgente );
                if(debugExec)	cout << "SQLiteConnection::exec: tmpValore=\'" << tmpValore << "\'" << endl;
                string valore = string( tmpValore );
                rs->righe.push_back( valore );
                delete[] tmpValore;
                //delete tmpSorgente;
            } else {
                rs->righe.push_back( string("\\N") );
            }
        }
        errorCode = sqlite3_step(pStmt);
    }
    if( errorCode!=SQLITE_DONE ) {
        cout << "SQLiteConnection::exec: errorCode = " << errorCode << endl;
        this->errorMessage.append(
                    "errorCode: "+ DBLayer::integer2string((long)errorCode) +
                    "; msg: " + sqlite3_errmsg(this->db)
                    );
        cout << "SQLiteConnection::exec: errorMessage = " << this->errorMessage << endl;
    }
    if(debugExec) cout << "SQLiteConnection::exec: Leggo tutte le righe... OK! " << endl;


    if(debugExec) cout << "SQLiteConnection::exec: finalizzo..." << endl;
    errorCode = sqlite3_finalize( pStmt );
    if(debugExec) cout << "SQLiteConnection::exec: finalizzo... OK" << endl;
    if( errorCode!=SQLITE_OK ) {
        cout << "SQLiteConnection::exec: errorCode = " << errorCode << endl;
        this->errorMessage.append(
                    "errorCode: "+ DBLayer::integer2string((long)errorCode) +
                    "; msg: " + sqlite3_errmsg(this->db)
                    );
        cout << "SQLiteConnection::exec: errorMessage = " << this->errorMessage << endl;
    } else {
        if(debugExec)	cout << "SQLiteConnection::exec: errorCode==SQLITE_OK" << endl;
    }
    //cout << "SQLiteConnection::exec: rs=" << rs->toString() << endl;
    //cout << "SQLiteConnection::exec: fine." << endl;
    return rs;
}

string SQLiteConnection::escapeString(string s) {
//    static string fromQuote("\'");
//    static string   toQuote("\'\'");
    return DBLayer::replaceAll(s, "\'", "\'\'");
}

int SQLiteConnection::getColumnSize(string* relname) {
	int ret=-1;
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
	
// 	cout << "SQLiteConnection::getColumnSize: query = " << query << endl;
	this->errorMessage.clear();
	
	errorCode = sqlite3_prepare(
	  this->db,               /* Database handle */
	  query,       /* SQL statement, UTF-8 encoded */
	  s.size(),             /* Length of zSql in bytes. */
	  &pStmt,  /* OUT: Statement handle */
	  &zTail     /* OUT: Pointer to unused portion of zSql */
	);
	
	if( errorCode!=SQLITE_OK ) {
		cout << "SQLiteConnection::getColumnSize: errorCode = " << errorCode << endl;
		this->errorMessage.append(
			"errorCode: "+ DBLayer::integer2string((long)errorCode) +
			"; msg: " + sqlite3_errmsg(this->db)
		);
		cout << "SQLiteConnection::getColumnSize: errorMessage = " << this->errorMessage << endl;
		cout << "SQLiteConnection::getColumnSize: error query = " << query << endl;
		return ret;
	}
	
	// Preparo i metadati
// 	cout << "SQLiteConnection::getColumnSize: Preparo i metadati " << endl;
	int nColonne = sqlite3_column_count(pStmt);
	ret = nColonne;
// 	cout << "SQLiteConnection::getColumnSize: finalizzo..." << endl;
	errorCode = sqlite3_finalize( pStmt );
// 	cout << "SQLiteConnection::getColumnSize: finalizzo... OK" << endl;
	if( errorCode!=SQLITE_OK ) {
		cout << "SQLiteConnection::getColumnSize: errorCode = " << errorCode << endl;
		this->errorMessage.append(
			"errorCode: "+ DBLayer::integer2string((long)errorCode) +
			"; msg: " + sqlite3_errmsg(this->db)
		);
		cout << "SQLiteConnection::getColumnSize: errorMessage = " << this->errorMessage << endl;
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
	
// 	cout << "SQLiteConnection::getColumnName: query = " << query << endl;
	this->errorMessage.clear();
	
	errorCode = sqlite3_prepare(
	  this->db,               /* Database handle */
	  query,       /* SQL statement, UTF-8 encoded */
	  s.size(),             /* Length of zSql in bytes. */
	  &pStmt,  /* OUT: Statement handle */
	  &zTail     /* OUT: Pointer to unused portion of zSql */
	);
	
	if( errorCode!=SQLITE_OK ) {
		cout << "SQLiteConnection::getColumnName: errorCode = " << errorCode << endl;
		this->errorMessage.append(
			"errorCode: "+ DBLayer::integer2string((long)errorCode) +
			"; msg: " + sqlite3_errmsg(this->db)
		);
		cout << "SQLiteConnection::getColumnName: errorMessage = " << this->errorMessage << endl;
		cout << "SQLiteConnection::getColumnName: error query = " << query << endl;
		return ret;
	}
	
	// Preparo i metadati
// 	cout << "SQLiteConnection::getColumnName: Preparo i metadati " << endl;
//	int nColonne = sqlite3_column_count(pStmt);
// 	cout << "SQLiteConnection::getColumnName: nColonne=" << DBLayer::integer2string((long)nColonne) << endl;
	// RRA - In SQLIte3 le colonne vanno da 0..n-1
	const char* nomeColonna = sqlite3_column_name(pStmt, column-1 );
	ret=string(nomeColonna);
	
// 	cout << "SQLiteConnection::getColumnName: finalizzo..." << endl;
	errorCode = sqlite3_finalize( pStmt );
// 	cout << "SQLiteConnection::getColumnName: finalizzo... OK" << endl;
	if( errorCode!=SQLITE_OK ) {
		cout << "SQLiteConnection::getColumnName: errorCode = " << errorCode << endl;
		this->errorMessage.append(
			"errorCode: "+ DBLayer::integer2string((long)errorCode) +
			"; msg: " + sqlite3_errmsg(this->db)
		);
		cout << "SQLiteConnection::getColumnName: errorMessage = " << this->errorMessage << endl;
	}
	return ret;
}
IntegerVector SQLiteConnection::getKeys(string* relname) {
	IntegerVector ret;
    cout << "SQLiteConnection::getKeys: UNSUPPORTED; relname=" << relname << endl;
	return ret;
}
IntegerVector SQLiteConnection::getForeignKeys(string* relname) {
	IntegerVector ret;
    cout << "SQLiteConnection::getForeignKeys: UNSUPPORTED; relname=" << relname << endl;
	return ret;
}
//********************* SQLiteConnection: fine.



//********************* SQLiteResultSet: inizio.

#ifdef WIN32
SQLiteResultSet::SQLiteResultSet() : ResultSet() {
#else
SQLiteResultSet::SQLiteResultSet() : ResultSet::ResultSet() {
#endif
}
SQLiteResultSet::~SQLiteResultSet() {
//	cout << "SQLiteResultSet::~SQLiteResultSet: inizio." << endl;
//	cout << "SQLiteResultSet::~SQLiteResultSet: fine." << endl;
}

int SQLiteResultSet::getColumnSize(int i) {
	// Non significativo per SQLite
    return -i;
//    return -1;
}

string SQLiteResultSet::toString(string prefix) {
	string ret;
	ret.append(prefix+"<SQLiteResultSet>");

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

	ret.append(prefix+"</SQLiteResultSet>");
	return ret;
};
//********************* SQLiteResultSet: fine.

#endif
