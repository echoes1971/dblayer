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
** @copyright &copy; 2011-2014 by Roberto Rocco Angeloni <roberto@roccoangeloni.it>
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

// for DBMgr.getNextUuid
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.

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
    if( this->verbose ) { cout << "DBMgr::~DBMgr: start." << endl; }
    if(this->_dbeuser!=0) {
        delete this->_dbeuser;
    }
    this->disconnect();
    if( this->verbose ) { cout << "DBMgr::~DBMgr: end." << endl; }
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
DBEntity* DBMgr::getClazz(const string& typeName) {
    if(this->dbeFactory!=0)
        return this->dbeFactory->getClazz(typeName);
    return new DBEntity(typeName);
}
DBEntity* DBMgr::getClazzByTypeName(const string& typeName) {
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
//        cout << "DBMgr::_buildWhereCondition: field=" << valore << endl;

        string clausola;
        string nome = field->getName();
        string valore = field->toString();
        cout << "DBMgr::_buildWhereCondition: valore=" << valore << endl;

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
        DBField* valore = (DBField*) dbe->getField( nomeCampo );
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
    for(const auto& elem : campi) {
        string nomeCampo = elem.first;
        DBField* field = (DBField*) elem.second;
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
        DBField* valore = (DBField*) dbe->getField(nomeChiave);
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
    if(this->con->isProxy()) {
        dbe = this->con->Insert(dbe);
        this->errorMessage.clear();
        this->errorMessage.append(this->con->getErrorMessage());
    } else {
        if( this->verbose ) cout << "DBMgr::Insert: dbe before insert = " << dbe->toString() << endl;
        // Insert
        string query = this->_buildInsertString(dbe);
        if( this->verbose ) cout << "DBMgr::Insert: query = " << query << endl;
        // Eseguo
        ResultSet* rs = this->con->exec( query );
        if( this->verbose ) cout << "DBMgr::Insert: status = " << rs->getStatus() << endl;
        delete rs;
    }
    // After Insert
    dbe = this->_after_insert(dbe);
    dbe->_after_insert(this);
    return dbe;
}
DBEntity* DBMgr::Update(DBEntity* dbe) {
    // Before Update
    dbe->_before_update(this);
    dbe = this->_before_update(dbe);
    if(this->con->isProxy()) {
        dbe = this->con->Update(dbe);
        this->errorMessage.clear();
        this->errorMessage.append(this->con->getErrorMessage());
    } else {
        if( this->verbose ) cout << "DBMgr::Update: dbe before update = " << dbe->toString() << endl;
        // Update
        string query = this->_buildUpdateString(dbe);
        if( this->verbose ) cout << "DBMgr::Update: query = " << query << endl;
        // Eseguo
        ResultSet* rs = this->con->exec( query );
        if( this->verbose ) cout << "DBMgr::Update: status = " << rs->getStatus() << endl;
        delete rs;
    }
    // After Update
    dbe = this->_after_update(dbe);
    dbe->_after_update(this);
    return dbe;
}
DBEntity* DBMgr::Delete(DBEntity* dbe) {
    // Before Delete
    dbe->_before_delete(this);
    dbe = this->_before_delete(dbe);
    if(this->con->isProxy()) {
        dbe = this->con->Delete(dbe);
        this->errorMessage.clear();
        this->errorMessage.append(this->con->getErrorMessage());
    } else {
        if( this->verbose ) cout << "DBMgr::Delete: dbe before delete = " << dbe->toString() << endl;
        // Delete
        string query = this->_buildDeleteString(dbe);
        if( this->verbose ) cout << "DBMgr::Delete: query = " << query << endl;
        // Eseguo
        ResultSet* rs = this->con->exec( query );
        if( this->verbose ) cout << "DBMgr::Delete: status = " << rs->getStatus() << endl;
        delete rs;
    }
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


void DBMgr::rs2dbelist(ResultSet* res,string& nomeTabella,DBEntityVector* ret) {
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
                dbfield = new BooleanField( columnName );
                dbfield->setBooleanValue( valore );
            } else if ( res->getColumnType(c)==DBLayer::type_integer ) {
                long valore = 0;
                valore = atoi( res->getValue( r, c ).c_str() );
                dbfield = new IntegerField( columnName, valore );
            } else if ( res->getColumnType(c)==DBLayer::type_double ) {
                float valore = 0;
                valore = (float) atof( res->getValue( r, c ).c_str() );
                dbfield = new FloatField( columnName, valore );
            } else if ( res->getColumnType(c)==DBLayer::type_string ) {
                string valore = res->getValue( r, c );
                dbfield = new StringField( columnName, valore );
            } else if ( res->getColumnType(c)==DBLayer::type_datetime ) {
                string valore = res->getValue( r, c );
                dbfield = new DateField( columnName, valore );
            }
            if(dbfield!=0) {
                dbe->addField(dbfield);
            }
        }
        ret->push_back( dbe );
    }
}

DBEntityVector* DBMgr::Select(const string& tableName, const string& searchString) {
    if(this->con->isProxy()) {
        DBEntityVector* ret = 0;
        string nomeTabella = tableName;
        if( this->verbose ) cout << "DBMgr::Select: nomeTabella=" << nomeTabella << endl;
        DBEntity* dbe = this->getClazz(nomeTabella);
        if( this->verbose ) cout << "DBMgr::Select: dbe=" << dbe->toString() << endl;
        string myTableName = this->_buildTableName(dbe);
        if( this->verbose ) cout << "DBMgr::Select: myTableName=" << myTableName << endl;
        ret = this->con->Select(dbe,myTableName,searchString);
        delete dbe;
        return ret;
    }
    DBEntityVector* ret = new DBEntityVector;
    ResultSet* res = this->con->exec(searchString);
    string nomeTabella = tableName;

    if (!con->hasErrors() ) {
        rs2dbelist(res, nomeTabella, ret);
    } else {
        if( this->verbose ) cout << "DBMgr::Select: error = " << con->getErrorMessage() << endl;
    }

    delete res;
    return ret;
}

DBEntityVector* DBMgr::Search(DBEntity* dbe, bool uselike, bool caseSensitive, const string &orderBy ) {
    if(this->verbose) cout << "DBMgr::Search: start." << endl;
    if(this->con->isProxy()) {
        if( this->verbose ) cout << "DBMgr::Search: calling con->Search()" << endl;
        //if( this->verbose ) cout << "DBMgr::Search: dbe=" << dbe->toString() << endl;
        return this->con->Search(dbe, uselike, caseSensitive, orderBy);
    }
    string myquery = this->_buildSelectString( dbe, uselike, caseSensitive );
    if( orderBy.size()!=0 ) {
        myquery.append( " ORDER BY " );
        myquery.append( orderBy.c_str() );
    }
    if( this->verbose ) cout << "DBMgr::Search: myquery = " << myquery << endl;
    string tableName = dbe->getTableName();
    if(this->verbose) cout << "DBMgr::Search: end." << endl;
    return this->Select(tableName,myquery);
}

DBEntityVector* DBMgr::searchByKeys(DBEntity* dbe) {
    DBEntity* cerca = dbe->createNewInstance();
    StringVector chiavi = dbe->getKeyNames();
    unsigned int chiaviSize = chiavi.size();
    for(unsigned int i=0; i<chiaviSize; i++) {
        Field* field = dbe->getField(chiavi[i]);
        string v = field->toString();
        if(field->isDate())
            cerca->setDateValue(chiavi[i],v);
        else
            cerca->setValue(chiavi[i],v);
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

string DBMgr::getNextUuid(DBEntity* dbe) {
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    string ret = boost::uuids::to_string(uuid);
    replaceAll(ret,"-","");
//        def getNextUuid(self, dbe, length=16):
//            return ( ("%s" % uuid.uuid4()).replace('-','') )[:16]
    return ret;
}

string DBMgr::ping() {
    if(this->con->isProxy())
        return this->con->ping();
    return "pong";
}

void DBMgr::_loadUserGroups() {
    if(this->_dbeuser==0)
        return;
    if(this->verbose) cout << "DBMgr::_loadUserGroups: start." << endl;
    DBEntity* cerca = this->getClazzByTypeName("DBEUserGroup");
    cerca->readFKFrom(this->_dbeuser);

    DBEntityVector* lista = this->Search(cerca,false);

    this->_user_groups_list.clear();
    for(const DBEntity* g : (*lista)) {
        this->_user_groups_list.push_back(string(g->getField("group_id")->getStringValue()->c_str()));
    }

    string user_group_id = string(this->_dbeuser->getField("group_id")->getStringValue()->c_str());
    vector<string>::iterator i = find(this->_user_groups_list.begin(), this->_user_groups_list.end(), user_group_id);
    if(i==this->_user_groups_list.end()) {
        this->_user_groups_list.push_back(user_group_id);
    }

    if(this->verbose) cout << "DBMgr::_loadUserGroups: loaded " << this->_user_groups_list.size() << " groups." << endl;

    this->Destroy(lista);
    delete cerca;
    if(this->verbose) cout << "DBMgr::_loadUserGroups: end." << endl;
}

DBEntity* DBMgr::login(const string user, const string pwd) {
    this->errorMessage.clear();

    if(this->con->isProxy()) {
        ResultSet* userRs = this->con->login(user,pwd);
        this->errorMessage = this->con->getErrorMessage();
        string nome_tabella("users");
        DBEntityVector list;

        this->rs2dbelist(userRs,nome_tabella,&list);

        if(list.size()!=1) {
            if( this->verbose ) { cout << "DBMgr::login: ERROR - list.size=" << list.size() << endl; }
            return 0;
        }
        this->_dbeuser = list.at(0);
        list.clear();

        this->_loadUserGroups();
        return this->_dbeuser;
    }
    if(pwd.length()==0 || user.length()==0) {
        if( this->verbose ) { cout << "DBMgr::login: ERROR - Missing username or password" << endl; }
        this->errorMessage = "Missing username or password";
        return this->_dbeuser;
    }
    DBEntity* cerca = this->getClazzByTypeName("DBEUser");
    cerca->setValue("login",user);
    cerca->setValue("pwd",pwd);
cout << "DBMgr::login: cerca=" << cerca->toString() << endl;
    DBEntityVector* lista = this->Search(cerca,false);

    // TODO: how to handle this in C++?
    //try:
    //    ret = self.search(cerca,uselike=False)
    //except Exception,e:
    //    try:
    //        self.initDB()
    //        newuser = self.getClazzByTypeName('DBEUser')(attrs={'login':user,'pwd':pwd})
    //        newuser = self.insert(newuser)
    //        searchGroup = self.getClazzByTypeName('DBEGroup')(attrs={'name':user})
    //        newgroup = self.search(searchGroup,uselike=False)[0]
    //        if self._verbose: print "DBMgr.login: newuser=%s" % ( newuser )
    //        newfolder = self.getClazzByTypeName('DBEFolder')(attrs={\
    //            'owner':newuser.getValue('id'),\
    //            'group_id':newgroup.getValue('id'),\
    //            'creator':newuser.getValue('id'),\
    //            'last_modify':newuser.getValue('id'),\
    //            'name':user})
    //        newfolder = self.insert(newfolder)
    //        if self._verbose: print "DBMgr.login: newfolder=%s" % ( newfolder )
    //        ret = self.search(cerca,uselike=False)

    if(lista->size()!=1) {
        this->errorMessage = "Wrong user or password";
        if( this->verbose ) { cout << "DBMgr::login: ERROR - lista.size=" << lista->size() << endl; }
        return 0;
    }
    this->_dbeuser = lista->at(0);
    lista->clear();
    this->Destroy(lista);

    this->_loadUserGroups();
    return this->_dbeuser;
}

DBEntity* DBMgr::relogin() {
    if(this->_dbeuser==0)
        return 0;
    string login(this->_dbeuser->getField("login")->getStringValue()->c_str());
    string pwd(this->_dbeuser->getField("pwd")->getStringValue()->c_str());
    return this->login(login,pwd);
}

bool DBMgr::isLoggedIn() { return this->_dbeuser!=0; }

string DBMgr::getServerIDString() {
    string d(this->getConnection()->getDBType());
    string u("nobody");
    if(this->_dbeuser!=0) {
        u = string(this->_dbeuser->getField("login")->getStringValue()->c_str());
    }
    string h(this->getConnection()->getConnectionString());
    // TODO
//    if d=="SQLite":
//        h = ( "%s" % self.getConnectionProvider().getDB() ).split( os.path.sep )[-1]
//    if h.find("://")>0:
//        _url = h.split("://")
//        if len(_url)>1:
//            h = "%s:%s" % ( _url[0],_url[1].split("/")[0] )
    string ret("");
    ret.append(u.c_str()); ret.append("@"); ret.append(h.c_str());
    return ret;
}

void DBMgr::addGroup(const string& group_id) {
    vector<string>::iterator i = find(this->_user_groups_list.begin(), this->_user_groups_list.end(), group_id);
    if(i==this->_user_groups_list.end()) {
        this->_user_groups_list.push_back(group_id);
    }
}

void DBMgr::Destroy(DBEntityVector* lista) {
    if(lista==0) return;
    for(DBEntity* elem : (*lista) ) {
        delete elem;
    }
    lista->clear();
    delete lista;
}

Connection* DBMgr::getConnection() { return this->con; }
void DBMgr::setConnection( Connection* _newVal){ this->con = _newVal; }

void DBMgr::setSchema(string schema) { this->_schema=schema; }
string DBMgr::getSchema() { return this->_schema; }
