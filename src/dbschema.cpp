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

//***********************	DBETestDBLayer: inizio.
const string DBETestDBLayer::nomiCampiChiave[] = { string("id") };
IntegerField DBETestDBLayer::chiave1( (const string*)&DBETestDBLayer::nomiCampiChiave[0] );
DBFieldVector DBETestDBLayer::chiavi( 1, &DBETestDBLayer::chiave1 );

DBETestDBLayer::DBETestDBLayer() {
	this->tableName.clear();
	this->tableName.append( "test_dblayer" );
}
DBETestDBLayer::~DBETestDBLayer() {}
string DBETestDBLayer::name() { return "DBETestDBLayer"; }
DBFieldVector* DBETestDBLayer::getKeys() { return &DBETestDBLayer::chiavi; }
DBETestDBLayer* DBETestDBLayer::createNewInstance() { return new DBETestDBLayer(); }
//***********************	DBETestDBLayer: inizio.


//***********************	DBESocieta: inizio.

const string DBESocieta::nomiCampiChiave[] = { string("id") };
IntegerField DBESocieta::chiave1( (const string*)&DBESocieta::nomiCampiChiave[0] );
DBFieldVector DBESocieta::chiavi( 1, &DBESocieta::chiave1 );


DBESocieta::DBESocieta() {
    this->tableName.clear();
    this->tableName.append( "societa" );
}
DBESocieta::~DBESocieta() {}
string DBESocieta::name() { return "DBESocieta"; }
DBFieldVector* DBESocieta::getKeys() { return &DBESocieta::chiavi; }
DBESocieta* DBESocieta::createNewInstance() { return new DBESocieta(); }

void DBESocieta::_before_insert(DBMgr* dbmgr) {
    // 2011.09.02: start.
    string query("select max(id) as max_id from societa");
    string nomeCampoDestinazione("id");
//    static string query("select max(id) as max_id from societa");
//    static string nomeCampo("max_id");
//    static string nomeCampoDestinazione("id");
    // 2011.09.02: end.
    string nomeTabella = this->name();
    DBEntityVector* lista = dbmgr->Select( &nomeTabella, &query );
    if( lista!=0 && lista->size()==1 ) {
        long maxId = 0;
        if(lista->at(0)->getFieldSize()>0) {
            DBField* field = (DBField*) lista->at(0)->getField(0);
            maxId = field->getIntegerValue();
        }
        this->setValue( &nomeCampoDestinazione, maxId+1 );

        dbmgr->Destroy(lista);
    }
    if(dbmgr==0) return; // Solo per togliere il warning di parametro non usato
}
void DBESocieta::_before_copy(DBMgr* dbmgr) {
	static string nomeCampo("ragione_sociale");
	DBField* campo = (DBField*) this->getField( &nomeCampo );
	if( campo==0 || campo->isNull() )
		return;
	string nuovoValore("Copy of ");
	nuovoValore.append( campo->toString() );
	campo->setStringValue( nuovoValore );
	//this->setValue( &nomeCampo, &nuovoValore );

    if(dbmgr==0) return; // Solo per togliere il warning di parametro non usato
}
//***********************	DBESocieta: inizio.
