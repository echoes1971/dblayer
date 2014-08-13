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
using namespace MySchema;

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
    string query("select max(id) as max_id from societa");
    string nomeTabella = this->name();
    DBEntityVector* lista = dbmgr->Select( &nomeTabella, &query );
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

string MySchema::getSchema() { return "test"; }
void MySchema::registerClasses(DBEFactory* dbeFactory) {
  dbeFactory->registerClass("test_dblayer", new DBETestDBLayer() );
  dbeFactory->registerClass("societa", new DBESocieta() );
}
