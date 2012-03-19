/***************************************************************************
**	dbmgr.cpp  v0.1.0 - 2012.03.19
**	-----------------------------------
**
**	Author:		Roberto Rocco Angeloni.
**	E-mail:		roberto@roccoangeloni.it
**	Comment:
**	To Do:
**	Future:
**	History:
**		v0.0.1 - 2002.10.23	First Old Version
**		v0.1.0 - 2006.05.04 Rewritten for the new framework
**
** @copyright &copy; 2011-2012 by Roberto Rocco Angeloni <roberto@roccoangeloni.it>
** @license http://opensource.org/licenses/lgpl-3.0.html GNU Lesser General Public License, version 3.0 (LGPLv3)
** @version $Id: dbmgr.cpp $
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

#include "dbmgr.h"
#include <typeinfo>
#include <new>
using namespace DBLayer;

DBLayer::DBMgr::DBMgr() {
    this->setConnection(0);
    this->dbeFactory = 0;
    this->_schema="";
    this->_dbeuser=0;
}

DBMgr::DBMgr(Connection* con, bool verbose) {
    this->name.append("DBMgr");
    this->setConnection(con);
    this->verbose = verbose;
    this->dbeFactory = 0;
    this->_schema="";
    this->_dbeuser=0;
}

DBMgr::~DBMgr() {
    if( this->verbose ) { cout << "DBMgr::~DBMgr: inizio." << endl; }
    this->disconnect();
    if( this->verbose ) { cout << "DBMgr::~DBMgr: fine." << endl; }
}

bool DBMgr::connect() {
    if ( !this->con->isConnected() ) {
        this->con->connect();
    }
    if ( !con->hasErrors() ) {
        this->errorMessage.clear();
        return true;
    }
    this->errorMessage = "Connection Error: " + this->con->getErrorMessage();
    return false;
}

bool DBMgr::disconnect() { return this->con==0 ? false : this->con->disconnect(); }

string DBMgr::getErrorMessage() { return this->errorMessage; }

void DBMgr::setDBEFactory(DBEFactory* dbeFactory) { this->dbeFactory=dbeFactory; }
DBEFactory* DBMgr::getDBEFactory() { return this->dbeFactory; }
void DBMgr::setVerbose(bool b) { this->verbose=b; }
DBEntityVector DBMgr::getRegisteredTypes() { return this->dbeFactory->getRegisteredTypes(); }
DBEntity* DBMgr::getClazz(string* typeName) {
    if(this->dbeFactory!=0)
        return this->dbeFactory->getClazz(typeName);
    return new DBEntity(typeName);
}
DBEntity* DBMgr::getClazzByTypeName(string* typeName) {
    if(this->dbeFactory!=0)
        return this->dbeFactory->getClazzByTypeName(typeName);
    return new DBEntity(typeName);
}

DBEntity* DBMgr::getDBEUser() { return this->_dbeuser; }
void DBMgr::setDBEUser(DBEntity* dbe) { this->_dbeuser=dbe; }

ColumnDefinitions DBMgr::getColumnsForTable(const string& tablename) {
    ColumnDefinitions ret;
    if(this->con==0) return ret;
    ret = this->con->getColumnsForTable(tablename);
    return ret;
}

string DBMgr::escapeString(string s) { return this->con->escapeString(s); }
string DBMgr::quoteDate(string s) { return this->con->quoteDate(s); }

DBLayer::StringVector DBMgr::_buildWhereCondition(DBEntity* dbe, bool uselike, bool caseSensitive) {
    StringVector clausole;
    int fieldSize = dbe->getFieldSize();
    for(int i=0; i<fieldSize; i++) {
        DBField* field = (DBField*) dbe->getField(i);
        if(this->verbose) {
            cout << "DBMgr::_buildWhereCondition: field->name=" << field->getName().c_str() << endl;
            cout << "DBMgr::_buildWhereCondition: field->isNull=" << field->isNull() << endl;
        }
        if( field==0 || field->isNull() )
            continue;
        string clausola;
        string nome = field->getName();
        string valore = field->toString();
        if( field->isString() ) {
            valore = this->escapeString(valore);
            if(uselike) {
                if(caseSensitive) {
                    clausola.append( nome )
#ifdef WIN32
                            .append( " like '%" )
                            .append( valore )
                            .append( "%'" );
#else
                            .append( " like '\%" )
                            .append( valore )
                            .append( "\%'" );
#endif
                } else {
#ifdef WIN32
                    clausola.append( "lower(" )
                            .append( nome )
                            .append( ") like '%" )
                            .append( lowerCase( valore ).c_str() )
                            .append( "%'" );
#else
                    clausola.append( "lower(" )
                            .append( nome )
                            .append( ") like '\%" )
                            .append( lowerCase( valore ).c_str() )
                            .append( "\%'" );
#endif
                }
            } else {
                if(caseSensitive) {
                    clausola.append( nome )
                            .append( "='" )
                            .append( valore )
                            .append( "'" );
                } else {
                    clausola.append( "lower(" )
                            .append( nome )
                            .append( ")='" )
                            .append( lowerCase( valore ).c_str() )
                            .append( "'" );
                }
            }
        } else if ( field->isDate() ) {
            valore = this->quoteDate(valore);
            clausola.append( nome )
                    .append( "=" )
                    .append( valore );
        } else {
            clausola.append( nome )
                    .append( "=" )
                    .append( valore );
        }
        clausole.push_back( clausola );
    }
    return clausole;
}
string DBMgr::_buildSelectString(DBEntity* dbe, bool uselike, bool caseSensitive) {
    string ret("select * from ");
    ret.append( this->_buildTableName(dbe) );
    StringVector clausole = this->_buildWhereCondition(dbe, uselike, caseSensitive);
    if( clausole.size()>0 ) {
        string myglue(" and ");
        string whereCondition = DBLayer::joinString( &clausole, &myglue );
        ret.append(" where ").append( whereCondition ).append(" ");
    }
    return ret;
}
string DBMgr::_buildInsertString(DBEntity* dbe) {
    string ret;

    StringVector nomiCampi = dbe->getNames();
    StringVector tmpNomi;
    StringVector tmpValori;
    for(unsigned int i=0; i<nomiCampi.size(); i++) {
        string nomeCampo = nomiCampi[i];
        DBField* valore = (DBField*) dbe->getField( &nomeCampo );
        if( valore==0 || valore->isNull() )
            continue;
        tmpNomi.push_back( nomeCampo );
        string valoreStringa = string( valore->toString() );
        string* valoreStringaPointer = &valoreStringa;

        if( valore->isString() ) {
            valoreStringa = this->escapeString( valoreStringa );
        }

        if( valore->isString() ) {
            valoreStringa.insert(0,"'");
            valoreStringa.append("'");
        } else if ( valore->isDate() ) {
            valoreStringa = this->quoteDate( valoreStringa );
        }
        tmpValori.push_back( valoreStringaPointer->c_str() );
    }

    string myglue(",");
    ret.append( "insert into " );
    ret.append( this->_buildTableName(dbe) );
    ret.append( " (" );
    ret.append( DBLayer::joinString(&tmpNomi,&myglue) );
    ret.append( ") values (" );
    ret.append( DBLayer::joinString(&tmpValori,&myglue) );
    ret.append( ")" );
    return ret;
}
string DBMgr::_buildUpdateString(DBEntity* dbe) {
    string ret;
    FieldMap campi = dbe->getValuesDictionary();
    StringVector setstring;
    for(FieldMap::iterator it=campi.begin(); it!=campi.end(); it++) {
        string nomeCampo = (*it).first;
        DBField* field = (DBField*) (*it).second;
        if( ! dbe->isKey( nomeCampo ) ) {
            string clausola;
            clausola.append( nomeCampo + "=" );
            if( !field->isNull() ) {
                string valoreStringa = string( field->toString() );
                if( field->isString() ) {
                    valoreStringa = this->escapeString( valoreStringa );
                    clausola.append("\'" + valoreStringa + "\'" );
                } else if ( field->isDate() ) {
                    valoreStringa = this->quoteDate( valoreStringa );
                    clausola.append( valoreStringa );
                } else {
                    clausola.append(valoreStringa);
                }
            } else {
                clausola.append("NULL");
            }
            setstring.push_back( clausola );
        }
    }
    string myglue(", ");
    ret.append( "update " + this->_buildTableName(dbe) );
    ret.append(   " set " + DBLayer::joinString(&setstring,&myglue) );
    ret.append( " where " + this->_buildKeysCondition(dbe) );
    return ret;
}
string DBMgr::_buildDeleteString(DBEntity* dbe) {
    string ret( "delete from " + this->_buildTableName(dbe) );
    ret.append( " where " + this->_buildKeysCondition(dbe) );
    return ret;
}
string DBMgr::_buildTableName(DBEntity* dbe) {
    string ret = "";
    if(this->_schema.length()>0) {
        ret.append(this->_schema);
        ret.append("_");
    }
    ret.append( dbe->getTableName() );
    return ret;
}
string DBMgr::_buildKeysCondition(DBEntity* dbe) {
    string ret;
    StringVector clausole;
    DBFieldVector* chiavi = dbe->getKeys();
    unsigned int chiaviSize = (unsigned int) chiavi->size();
    for(unsigned int i=0; i<chiaviSize; i++) {
        DBField* chiave = chiavi->at(i);
        string nomeChiave = chiave->getName();
        DBField* valore = (DBField*) dbe->getField(&nomeChiave);
        if( valore!=0 && !valore->isNull() ) {
            string valoreChiave = valore->toString();
            string clausola;
            if ( valore->isString() ) {
                valoreChiave = this->escapeString( valoreChiave );
                clausola.append( nomeChiave );
                clausola.append( "='" );
                clausola.append( valoreChiave );
                clausola.append( "'" );
            } else if ( valore->isDate() ) {
                valoreChiave = this->quoteDate( valoreChiave );
                clausola.append( nomeChiave );
                clausola.append( "=" );
                clausola.append( valoreChiave );
            } else {
                clausola.append( nomeChiave );
                clausola.append( "=" );
                clausola.append( valoreChiave );
            }
            clausole.push_back( clausola );
        }
    }
    string myglue(" AND ");
    ret = DBLayer::joinString( &clausole, &myglue );
    return ret;
}

DBEntity* DBMgr::_before_insert(DBEntity* dbe) { return dbe; }
DBEntity* DBMgr::_after_insert(DBEntity* dbe) { return dbe; }
DBEntity* DBMgr::_before_update(DBEntity* dbe) { return dbe; }
DBEntity* DBMgr::_after_update(DBEntity* dbe) { return dbe; }
DBEntity* DBMgr::_before_delete(DBEntity* dbe) { return dbe; }
DBEntity* DBMgr::_after_delete(DBEntity* dbe) { return dbe; }
DBEntity* DBMgr::_before_copy(DBEntity* dbe) { return dbe; }
DBEntity* DBMgr::_after_copy(DBEntity* dbe) { return dbe; }

DBEntity* DBMgr::Insert(DBEntity* dbe) {
    // Before Insert
    dbe->_before_insert(this);
    dbe = this->_before_insert(dbe);
#ifdef WITH_PROXY_CONNECTIONS
    if(this->con->isProxy()) {
        dbe = this->con->Insert(dbe);
        this->errorMessage.clear();
        this->errorMessage.append(this->con->getErrorMessage());
    } else {
#endif
        if( this->verbose ) cout << "DBMgr::Insert: dbe before insert = " << dbe->toString() << endl;
        // Insert
        string query = this->_buildInsertString(dbe);
        if( this->verbose ) cout << "DBMgr::Insert: query = " << query << endl;
        // Eseguo
        ResultSet* rs = this->con->exec( query );
        if( this->verbose ) cout << "DBMgr::Insert: status = " << rs->getStatus() << endl;
        delete rs;
#ifdef WITH_PROXY_CONNECTIONS
    }
#endif
    // After Insert
    dbe = this->_after_insert(dbe);
    dbe->_after_insert(this);
    return dbe;
}
DBEntity* DBMgr::Update(DBEntity* dbe) {
    // Before Update
    dbe->_before_update(this);
    dbe = this->_before_update(dbe);
#ifdef WITH_PROXY_CONNECTIONS
    if(this->con->isProxy()) {
        dbe = this->con->Update(dbe);
        this->errorMessage.clear();
        this->errorMessage.append(this->con->getErrorMessage());
    } else {
#endif
        if( this->verbose ) cout << "DBMgr::Update: dbe before update = " << dbe->toString() << endl;
        // Update
        string query = this->_buildUpdateString(dbe);
        if( this->verbose ) cout << "DBMgr::Update: query = " << query << endl;
        // Eseguo
        ResultSet* rs = this->con->exec( query );
        if( this->verbose ) cout << "DBMgr::Update: status = " << rs->getStatus() << endl;
        delete rs;
#ifdef WITH_PROXY_CONNECTIONS
    }
#endif
    // After Update
    dbe = this->_after_update(dbe);
    dbe->_after_update(this);
    return dbe;
}
DBEntity* DBMgr::Delete(DBEntity* dbe) {
    // Before Delete
    dbe->_before_delete(this);
    dbe = this->_before_delete(dbe);
#ifdef WITH_PROXY_CONNECTIONS
    if(this->con->isProxy()) {
        dbe = this->con->Delete(dbe);
        this->errorMessage.clear();
        this->errorMessage.append(this->con->getErrorMessage());
    } else {
#endif
        if( this->verbose ) cout << "DBMgr::Delete: dbe before delete = " << dbe->toString() << endl;
        // Delete
        string query = this->_buildDeleteString(dbe);
        if( this->verbose ) cout << "DBMgr::Delete: query = " << query << endl;
        // Eseguo
        ResultSet* rs = this->con->exec( query );
        if( this->verbose ) cout << "DBMgr::Delete: status = " << rs->getStatus() << endl;
        delete rs;
#ifdef WITH_PROXY_CONNECTIONS
    }
#endif
    // After Delete
    dbe = this->_after_delete(dbe);
    dbe->_after_delete(this);
    return dbe;
}
DBEntity* DBMgr::Copy(DBEntity* dbe) {
    // 1. Read the full DBE
    DBEntityVector* tmp = this->Search( dbe, false, true );
    if( tmp==0 || tmp->size()!=1 ) {
        delete tmp;
        return 0;
    }
    DBEntity* mydbe = tmp->at(0);
    delete tmp;

    // 2. Cleaning key fields
    mydbe->cleanKeyFields();

    // Before Copy
    mydbe->_before_copy(this);
    mydbe = this->_before_copy(mydbe);
    if( this->verbose ) { cout << "DBMgr::Copy: mydbe before copy = " << mydbe->toString() << endl; }
    // Copy (Insert)
    mydbe = this->Insert(mydbe);
    // After Copy
    mydbe = this->_after_copy(mydbe);
    mydbe->_after_copy(this);
    return mydbe;
}


DBEntityVector* DBMgr::Select(const string* tableName, const string* searchString) {
#ifdef WITH_PROXY_CONNECTIONS
    if(this->con->isProxy()) {
        DBEntityVector* ret = 0;
        string* nomeTabella = new string( tableName->c_str() );
        DBEntity* dbe = this->getClazz(nomeTabella);
        string myTableName = this->_buildTableName(dbe);
        ret = this->con->Select(dbe,&myTableName,searchString);
        delete dbe;
        delete nomeTabella;
        return ret;
    }
#endif
    DBEntityVector* ret = new DBEntityVector;
    ResultSet* res = this->con->exec( string(searchString->c_str()) );
    string* nomeTabella = new string( tableName->c_str() );

    if (!con->hasErrors() ) {
        int nColonne = res->getNumColumns();
        int nRighe = res->getNumRows();
        for(int r=0; r<nRighe; r++) {
            DBEntity* dbe = this->getClazz(nomeTabella);
            for(int c=0; c<nColonne; c++) {
                DBField* dbfield = 0;
                if ( res->isNull( r, c ) ) {
                    continue;
                }
                string columnName = res->getColumnName(c);
                if ( res->getColumnType(c)==DBLayer::type_boolean ) {
                    bool valore = false;
                    valore = res->getValue( r, c ) == string("t");
                    dbfield = new BooleanField( &columnName );
                    dbfield->setBooleanValue( valore );
                } else if ( res->getColumnType(c)==DBLayer::type_integer ) {
                    long valore = 0;
                    valore = atoi( res->getValue( r, c ).c_str() );
                    dbfield = new IntegerField( &columnName, valore );
                } else if ( res->getColumnType(c)==DBLayer::type_double ) {
                    float valore = 0;
                    valore = (float) atof( res->getValue( r, c ).c_str() );
                    dbfield = new FloatField( &columnName, valore );
                } else if ( res->getColumnType(c)==DBLayer::type_string ) {
                    string valore = res->getValue( r, c );
                    dbfield = new StringField( &columnName, &valore );
                } else if ( res->getColumnType(c)==DBLayer::type_datetime ) {
                    string valore = res->getValue( r, c );
                    dbfield = new DateField( &columnName, &valore );
                }
                if(dbfield!=0) {
                    dbe->addField(dbfield);
                }
            }
            ret->push_back( dbe );
        }
    } else {
        if( this->verbose ) cout << "DBMgr::Select: error = " << con->getErrorMessage() << endl;
    }

    delete nomeTabella;
    delete res;
    return ret;
}

DBEntityVector* DBMgr::Search(DBEntity* dbe, bool uselike, bool caseSensitive, const string* orderBy ) {
#ifdef WITH_PROXY_CONNECTIONS
    if(this->con->isProxy()) {
        //if( this->verbose ) cout << "DBMgr::Search: calling con->Search()" << endl;
        return this->con->Search(dbe, uselike, caseSensitive, orderBy);
    }
#endif
    string myquery = this->_buildSelectString( dbe, uselike, caseSensitive );
    if( orderBy->size()!=0 ) {
        myquery.append( " ORDER BY " );
        myquery.append( orderBy->c_str() );
    }
    if( this->verbose ) cout << "DBMgr::Search: myquery = " << myquery << endl;
    string tableName = dbe->getTableName();
    return this->Select( &tableName, &myquery );
}

DBEntityVector* DBMgr::searchByKeys(DBEntity* dbe) {
    DBEntity* cerca = dbe->createNewInstance();
    StringVector chiavi = dbe->getKeyNames();
    unsigned int chiaviSize = chiavi.size();
    for(unsigned int i=0; i<chiaviSize; i++) {
        Field* field = dbe->getField(&chiavi[i]);
        string v = field->toString();
        if(field->isDate())
            cerca->setDateValue(&chiavi[i],&v);
        else
            cerca->setValue(&chiavi[i],&v);
    }
    return this->Search(cerca,false,false);
}
bool DBMgr::exists(DBEntity* dbe) {
    if(dbe->isNew())
        return false;
    DBEntityVector* tmp = this->searchByKeys(dbe);
    int count = tmp->size();
    this->Destroy(tmp);
    return count>0;
}

void DBMgr::Destroy(DBEntityVector* lista) {
    if(lista==0) return;
    DBEntityVector::iterator theIterator;
    for(theIterator = lista->begin(); theIterator!=lista->end(); theIterator++) {
        delete (DBEntity*) (*theIterator);
    }
    lista->clear();
    delete lista;
}

Connection* DBMgr::getConnection() { return this->con; }
void DBMgr::setConnection( Connection* _newVal){ this->con = _newVal; }

void DBMgr::setSchema(string schema) { this->_schema=schema; }
string DBMgr::getSchema() { return this->_schema; }
