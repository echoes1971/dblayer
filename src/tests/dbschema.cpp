/***************************************************************************
**	dbschema.cpp  v0.0.1 - 2006.05.04
**	---------------------------------
**
**	Author:		Roberto Rocco Angeloni.
**	email:		roberto@roccoangeloni.it
**	Copyright:	(c) 2006 by Roberto Rocco Angeloni.
**	Comment:
**	To Do:
**	Future:
**	History:
**		v0.0.1 - 2006.04.XX	...
****************************************************************************/

#include "dbschema.h"
using namespace TestSchema;

#include <string>
using namespace std;

//***********************	DBETestDBLayer: start.
const string DBETestDBLayer::nomiCampiChiave[] = { string("id") };
ColumnDefinitions DBETestDBLayer::_columns;
ColumnDefinitions DBETestDBLayer::getColumns() const { return DBETestDBLayer::_columns; }
IntegerField DBETestDBLayer::chiave1( DBETestDBLayer::nomiCampiChiave[0] );
DBFieldVector DBETestDBLayer::chiavi( 1, &DBETestDBLayer::chiave1 );

DBETestDBLayer::DBETestDBLayer() {
	this->tableName.clear();
	this->tableName.append( "test_dblayer" );
    this->schemaName = TestSchema::getSchema();
    if(DBETestDBLayer::_columns.size()==0) {
        for(const pair<string,vector<string> > pair: DBEntity::getColumns()) {
            DBETestDBLayer::_columns[pair.first] = pair.second;
        }
        DBETestDBLayer::_columns["id"] = vector<string> {"uuid","not null"};
        DBETestDBLayer::_columns["nome"] = vector<string> {"varchar(255)"};
        DBETestDBLayer::_columns["descrizione"] = vector<string> {"varchar(2000)"};
        DBETestDBLayer::_columns["abilitato"] = vector<string> {"bool"};
        DBETestDBLayer::_columns["data_creazione"] = vector<string> {"datetime"};
        DBETestDBLayer::_columns["prezzo"] = vector<string> {"float"};
        DBETestDBLayer::_columns["data_disponibilita"] = vector<string> {"datetime"};
    }

//    id int NOT NULL,
//    nome text,
//    descrizione text,
//    abilitato int,
//    data_creazione text,
//    prezzo float,
//    data_disponibilita text,
//    PRIMARY KEY (id)
}
DBETestDBLayer::~DBETestDBLayer() {}
string DBETestDBLayer::name() const { return "DBETestDBLayer"; }
DBFieldVector* DBETestDBLayer::getKeys() const { return &DBETestDBLayer::chiavi; }
DBETestDBLayer* DBETestDBLayer::createNewInstance() const { return new DBETestDBLayer(); }

void DBETestDBLayer::_before_insert(DBMgr* dbmgr) {
    if(this->isNull("id")) {
        this->setValue("id",dbmgr->getNextUuid(this));
    }
}

//***********************	DBETestDBLayer: end.


//***********************	DBESocieta: start.

const string DBESocieta::nomiCampiChiave[] = { string("id") };
ColumnDefinitions DBESocieta::_columns;
ColumnDefinitions DBESocieta::getColumns() const { return DBESocieta::_columns; }
IntegerField DBESocieta::chiave1( DBESocieta::nomiCampiChiave[0] );
DBFieldVector DBESocieta::chiavi( 1, &DBESocieta::chiave1 );
DBESocieta::DBESocieta() {
    this->tableName.clear();
    this->tableName.append( "societa" );
    this->schemaName = TestSchema::getSchema();
    if(DBESocieta::_columns.size()==0) {
        for(const pair<string,vector<string> > pair: DBEntity::getColumns()) {
            DBESocieta::_columns[pair.first] = pair.second;
        }
        DBESocieta::_columns["id"] = vector<string> {"uuid","not null"};
        DBESocieta::_columns["ragione_sociale"] = vector<string> {"text"};
        DBESocieta::_columns["indirizzo"] = vector<string> {"text"};
        DBESocieta::_columns["cap"] = vector<string> {"varchar(6)"};
        DBESocieta::_columns["nazione"] = vector<string> {"text"};
        DBESocieta::_columns["telefono"] = vector<string> {"text"};
        DBESocieta::_columns["fax"] = vector<string> {"text"};
        DBESocieta::_columns["email"] = vector<string> {"text"};
        DBESocieta::_columns["note"] = vector<string> {"text"};
        DBESocieta::_columns["website"] = vector<string> {"text"};
        DBESocieta::_columns["citta"] = vector<string> {"text"};
        DBESocieta::_columns["provincia"] = vector<string> {"text"};
        DBESocieta::_columns["partita_iva"] = vector<string> {"text"};
        DBESocieta::_columns["tipo"] = vector<string> {"text"};
        DBESocieta::_columns["data_creazione"] = vector<string> {"datetime"};
    }
//    id integer NOT NULL,
//    ragione_sociale text,
//    indirizzo text,
//    cap integer,
//    nazione text,
//    telefono integer,
//    fax integer,
//    email text,
//    note text,
//    website text,
//    citta text,
//    provincia text,
//    partita_iva text,
//    tipo text,
//    data_creazione text,
}
DBESocieta::~DBESocieta() {}
string DBESocieta::name() const { return "DBESocieta"; }
DBFieldVector* DBESocieta::getKeys() const { return &DBESocieta::chiavi; }
DBESocieta* DBESocieta::createNewInstance() const { return new DBESocieta(); }

void DBESocieta::_before_insert(DBMgr* dbmgr) {
    if(this->isNull("id")) {
        this->setValue("id",dbmgr->getNextUuid(this));
    }
}
void DBESocieta::_before_copy(DBMgr* dbmgr) {
    DBField* campo = (DBField*) this->getField("ragione_sociale");
	if( campo==0 || campo->isNull() )
		return;
	string nuovoValore("Copy of ");
	nuovoValore.append( campo->toString() );
	campo->setStringValue( nuovoValore );

    if(dbmgr==0) return;
}
//***********************	DBESocieta: end.

string TestSchema::getSchema() { return "test"; }
void TestSchema::registerClasses(DBEFactory* dbeFactory) {
  dbeFactory->registerClass("test_dblayer", new DBETestDBLayer() );
  dbeFactory->registerClass("societa", new DBESocieta() );
}
void TestSchema::checkDB(DBMgr& dbmgr, bool verbose) {
    if(verbose) cout << "TestSchema::checkDB: start." << endl;

    if(!dbmgr.connect()) {
        cerr << "TestSchema::checkDB: ERROR - UNABLE TO CONNECT TO DB!!!" << endl;
        return;
    }

    // 1. Check app version
    long current_db_version = -1;
    DBEntity* dbecurrentversion;
    DBEntity* cerca = dbmgr.getClazz("dbversion");
    cerca->setValue("model_name", TestSchema::getSchema());
    DBEntityVector* res = dbmgr.Search(cerca,false);
    if(verbose) cout << "TestSchema::checkDB: res.size=" << res->size() << endl;
    if(dbmgr.getErrorMessage().length()==0 && res->size()>0) {
        dbecurrentversion = res->at(0);
        res->clear();
        if(verbose) cout << "TestSchema::checkDB: " << dbecurrentversion->toString("\n") << endl;
        current_db_version = dbecurrentversion->getIntegerValue("version");
    } else if(dbmgr.getErrorMessage().length()>0) {
        cerr << "TestSchema::checkDB: " << dbmgr.getErrorMessage() << endl;
    }
    dbmgr.Destroy(res);
    delete cerca;

    if(verbose) cout << "TestSchema::checkDB: current_db_version=" << current_db_version << endl;

    // Lambda :-)
    std::function<string(const string&)> lambda_dbeType2dbType = [&dbmgr] (const string& s) mutable -> string { return dbmgr.getConnection()->dbeType2dbType(s); };

    // drop table test_test_dblayer,test_societa;
    // drop table dblayer_dbversion,auth_groups,auth_users,auth_users_groups, test_test_dblayer,test_societa;

    // 2. Do the DB migration
    long current_migration = -1;
    if(current_db_version<0) {
        string sql;
        bool use_fk = dbmgr.getConnection()->getDBType()!="MYSQL";// && dbmgr.getConnection()->getDBType()!="SQLite";

        dbecurrentversion = dbmgr.getClazzByTypeName("DBEDBVersion");
        dbecurrentversion->setValue("model_name", TestSchema::getSchema());
        dbecurrentversion->setValue("version",current_db_version);
        dbecurrentversion = dbmgr.Insert(dbecurrentversion);
        if(verbose) cout << dbecurrentversion->toString("\n") << endl;

        DBETestDBLayer dbe1;
        sql = dbe1.toSql(lambda_dbeType2dbType,"\n",use_fk);
        dbmgr.getConnection()->exec(sql);
        if(verbose) cout << sql << endl;

        DBESocieta dbe2;
        sql = dbe2.toSql(lambda_dbeType2dbType,"\n",use_fk);
        dbmgr.getConnection()->exec(sql);
        if(verbose) cout << sql << endl;

        dbe1.setNull("id")->setValue("nome","cippa")->setValue("descrizione","Cippa Lippa")
            ->setValue("data_creazione","2006-04-01 16:15:30")
            ->setValue("abilitato",true)
            ->setValue("prezzo",6.023F); dbmgr.Insert(&dbe1);
        dbe1.setNull("id")->setValue("nome","lippo")->setValue("descrizione","Lippo Lippi")
            ->setValue("data_creazione","2006-04-01 16:15:30")
            ->setValue("abilitato",false)
            ->setValue("prezzo",(float) -125); dbmgr.Insert(&dbe1);
        dbe1.setNull("id")->setValue("nome","generale")->setValue("descrizione","Generale Putzerstoven")
            ->setValue("data_creazione","2014-04-01 16:15:30")
            ->setValue("abilitato",true)
            ->setValue("prezzo",(float) 99); dbmgr.Insert(&dbe1);

        dbe2.setNull("id")->setValue("ragione_sociale","Società A Srl")->setValue("indirizzo","via vai 7, falconara")
                ->setValue("cap",60015L)->setValue("nazione","IT"); dbmgr.Insert(&dbe2);
        dbe2.setNull("id")->setValue("ragione_sociale","Società B Srl")->setValue("indirizzo","via matteotti 7, falconara")
                ->setValue("cap",60015L)->setValue("nazione","IT"); dbmgr.Insert(&dbe2);
//    telefono integer,
//    fax integer,
//    email text,
//    note text,
//    website text,
//    citta text,
//    provincia text,
//    partita_iva text,
//    tipo text,
//    data_creazione text,
    }
    current_migration++;

    // 3. Write version to DB
    dbecurrentversion->setValue("version",current_migration);
    dbmgr.Update(dbecurrentversion);

    delete dbecurrentversion;

    if(verbose) cout << "TestSchema::checkDB: end." << endl;
}
