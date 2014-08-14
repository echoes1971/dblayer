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
IntegerField DBETestDBLayer::chiave1( DBETestDBLayer::nomiCampiChiave[0] );
DBFieldVector DBETestDBLayer::chiavi( 1, &DBETestDBLayer::chiave1 );

DBETestDBLayer::DBETestDBLayer() {
	this->tableName.clear();
	this->tableName.append( "test_dblayer" );
}
DBETestDBLayer::~DBETestDBLayer() {}
string DBETestDBLayer::name() const { return "DBETestDBLayer"; }
DBFieldVector* DBETestDBLayer::getKeys() const { return &DBETestDBLayer::chiavi; }
DBETestDBLayer* DBETestDBLayer::createNewInstance() const { return new DBETestDBLayer(); }
//***********************	DBETestDBLayer: end.


//***********************	DBESocieta: start.

const string DBESocieta::nomiCampiChiave[] = { string("id") };
IntegerField DBESocieta::chiave1( DBESocieta::nomiCampiChiave[0] );
DBFieldVector DBESocieta::chiavi( 1, &DBESocieta::chiave1 );


DBESocieta::DBESocieta() {
    this->tableName.clear();
    this->tableName.append( "societa" );
}
DBESocieta::~DBESocieta() {}
string DBESocieta::name() const { return "DBESocieta"; }
DBFieldVector* DBESocieta::getKeys() const { return &DBESocieta::chiavi; }
DBESocieta* DBESocieta::createNewInstance() const { return new DBESocieta(); }

void DBESocieta::_before_insert(DBMgr* dbmgr) {
    DBEntityVector* lista = dbmgr->Select(this->name(),"select max(id) as max_id from societa");
    if( lista!=0 && lista->size()==1 ) {
        long maxId = 0;
        if(lista->at(0)->getFieldSize()>0) {
            DBField* field = (DBField*) lista->at(0)->getField(0);
            maxId = field->getIntegerValue();
        }
        this->setValue("id", maxId+1 );
        dbmgr->Destroy(lista);
    }
    if(dbmgr==0) return;
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
void TestSchema::checkDB(DBMgr& dbmgr) {
    cout << "TestSchema::checkDB: start." << endl;

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
    cout << "TestSchema::checkDB: res.size=" << res->size() << endl;
    if(dbmgr.getErrorMessage().length()==0 && res->size()>0) {
        dbecurrentversion = res->at(0);
        cout << "TestSchema::checkDB: " << dbecurrentversion->toString("\n") << endl;
        current_db_version = dbecurrentversion->getIntegerValue("version");
    } else if(dbmgr.getErrorMessage().length()>0) {
        cout << "TestSchema::checkDB: " << dbmgr.getErrorMessage() << endl;
    }
    dbmgr.Destroy(res);
    delete cerca;

    cerr << "TestSchema::checkDB: current_db_version=" << current_db_version << endl;

    // drop table test_test_dblayer,test_societa;

    // 2. Do the DB migration
    long current_migration = -1;
    if(current_db_version<0) {
        string sql;
        bool use_fk = dbmgr.getConnection()->getDBType()!="MYSQL" && dbmgr.getConnection()->getDBType()!="SQLite";

        dbecurrentversion = dbmgr.getClazzByTypeName("DBEDBVersion");
        dbecurrentversion->setValue("model_name", TestSchema::getSchema());
        dbecurrentversion->setValue("version",current_db_version);
        dbecurrentversion = dbmgr.Insert(dbecurrentversion);
        cout << dbecurrentversion->toString("\n") << endl;

        DBETestDBLayer dbe1;
        sql = dbe1.toSql("\n",use_fk);
        dbmgr.getConnection()->exec(sql);
        cout << sql << endl;

        DBESocieta dbe2;
        sql = dbe2.toSql("\n",use_fk);
        dbmgr.getConnection()->exec(sql);
        cout << sql << endl;

//        dbeusergroup.setValue("user_id","-4")->setValue("group_id","-1"); dbmgr.Insert(&dbeusergroup);
//        dbeusergroup.setValue("user_id","-5")->setValue("group_id","-2"); dbmgr.Insert(&dbeusergroup);
//        dbeusergroup.setValue("user_id","-6")->setValue("group_id","-3"); dbmgr.Insert(&dbeusergroup);

    }
    current_migration++;

    // 3. Write version to DB
    dbecurrentversion->setValue("version",current_migration);
    cout << dbecurrentversion->toString("\n") << endl;
    dbmgr.Update(dbecurrentversion);

    delete dbecurrentversion;

    cout << "TestSchema::checkDB: end." << endl;
}
