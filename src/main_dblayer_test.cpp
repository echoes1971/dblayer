/** *************************************************************************
**	main.cpp  v0.0.1 - 2006.04.03
**	-----------------------------
**
**	Author:		Roberto Rocco Angeloni.
**	E-mail:			roberto@roccoangeloni.it
**	Copyright:	(c) 2011 by Roberto Rocco Angeloni.
**	Comment:
**		dblayer:odbc:roberto
**		dblayer:odbc:test_dblayer
**		"dblayer:pg:host=localhost dbname=roberto user=roberto password=roberto"
**		dblayer:sqlite:F:\Roberto\CVS_Workspace\CPP\db_layer\db_layer2\test.db
**		dblayer:sqlite:./test.db
**		dblayer:xmlrpc:http://localhost/~roberto/rproject/xmlrpc_server.php
**		dblayer:xmlrpc:http://www.roccoangeloni.it/rproject/xmlrpc_server.php
**		"dblayer:mysql:host=localhost;dbname=roberto;user=root;password=;"
**	TODO: - Controllare che tutti i metodi virtual della classe DBConnection abbiano implementazione nelle sottoclassi
**				PG: OK
**				MYSQL: OK
**				ODBC:
**				SQLITE: parziale, non e' possibile implementare la lettura chiavi e chiavi esterne
**			- Aggiungere DBConnection::getForeignKeys(rel_name) ed il relativo test
**				PG:
**				MYSQL: OK
**				ODBC:
**				SQLITE: NO, vedi sopra
**			- I Field specializzati ed i DBField specializzati sembrano avere le stesse funzionalità... ACCORPARE?
**			- Aggiungere a DBMgr la possibilità di <b>creare</b> tabelle in base alle DBE fornite dalla factory
**			- Aggiungere a DBMgr la possibilità di <b>aggiornare</b> tabelle in base alle DBE fornite dalla factory
**			- Aggiungere a DBMgr la possibilità di <b>eliminare</b> tabelle in base alle DBE fornite dalla factory
**			- XmlrpcConnection: interfaccia verso il PHP-DBLayer
**	Future:
**	History:
**		v1.0 - 2006.03.31	...
*************************************************************************** */


#include <cstdio>
#include <cmath>

#include "dblayer/dbconnection.h"
#include "dblayer/dbmgr.h"
#include "dblayer/datefield.h"
using namespace DBLayer;

#include "dbschema.h"
using namespace MySchema;

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#if defined(WIN32)
# define snprintf _snprintf
#endif

#include <iostream>
#include <stdlib.h>
#include <typeinfo>

#define ASSERT(x) \
do { \
  if(!(x)) { \
    cerr << "Assertion \"" << #x << "\" failed" << endl; \
    assertionsFailed++; \
  } \
} while(false)

using namespace std;

void testDBConnection(string& connString) {
    DBLayer::Connection* con;
    DBLayer::ResultSet* res;

    con = DBLayer::createConnection( connString.c_str() );
    con->connect();

    if ( !con->hasErrors() ) {
        string myquery("select * from rra_users order by id desc");
        res = con->exec(myquery);
        //res = con->exec(string("select * from kware_users order by id desc"));
        //res = con->exec(string("select * from test_dblayer order by id desc"));
        //res = con->exec(string("select * from societa"));
        //res = con->exec(string("select oid,typname from pg_type"));
        //string relname = string("comuni");
        //res = con->exec(string("select conkey from pg_constraint join pg_class on pg_class.oid=conrelid where contype='p' and relname='" + relname + "' "));

        if( !con->hasErrors() ) {
            cout << "res.status: " << res->getStatus() << endl;
            cout << "res.toString() = " << res->toString() << endl;

            int nColonne = res->getNumColumns();
            printf( "N. Colonne: %d\n", nColonne );
            for( int i=0; i<nColonne; i++) {
                cout << "Colonna[" << i << "]: " << string(res->getColumnName(i))
                     << " - " << res->getColumnType(i)
                     << ": " << res->getColumnSize(i)
                     << endl;
            }

            int nRighe = res->getNumRows();
            cout << " Righe:" << nRighe << endl;
            for(int r=0; r<nRighe; r++) {
                for(int c=0; c<nColonne; c++) {
                    if (! res->isNull(r,c) ) {
                        cout << res->getValue(r,c) << "\t";
                    } else {
                        cout << "\\N" << "\t";
                    }
                }
                cout << endl;
            }
        } else {
            cout << "Errori: " << con->getErrorMessage() << endl;
        }
        delete res;
    } else {
        cout << "(testDBConnection) Errori: " << con->getErrorMessage() << endl;
    }
    //con->disconnect();
    delete con;
}
void testDBConnection(string host,string dbname,string usr,string pwd) {
    string connString = string( "host="+host+" dbname="+dbname+" user="+usr+" password="+pwd );
    testDBConnection( connString );
}

void testDateField() {
    string* tmpNome = new string("cippa");
    string* tmpData = new string("1970-02-01 00:00:00");
    string* tmpData2 = new string("2038-01-19 00:00:00");
    DBLayer::DateField dateField = DBLayer::DateField( tmpNome, tmpData );
    cout << "string: " << dateField.toString() << endl;
    cout << "seconds: " << dateField.to_seconds() << endl;
    cout << "days: " << dateField.to_days() << endl;

    dateField.setValue( dateField.to_seconds() );
    cout << "seconds2date: " << dateField.toString() << endl;

    dateField.setValue( tmpData2 );
    cout << "date: " << dateField.toString() << endl;
    dateField.setValue( dateField.to_seconds() );
    cout << "seconds2date: " << dateField.toString() << endl;

    delete tmpData2;
    delete tmpData;
    delete tmpNome;
}

void testGetKeys(string connString, string relname) {
    DBLayer::Connection* con;

    con = DBLayer::createConnection( connString.c_str() );
    con->connect();

    if ( !con->hasErrors() ) {
        cout << "Tabella: " << relname << endl;

        int numColonne = con->getColumnSize( &relname );
        cout << "\tNum. Colonne: " << numColonne << endl;
        for(int c = 1; c<=numColonne; c++) {
            cout << "\t" << c << ") " << con->getColumnName( &relname, c ) << endl;
        }

        DBLayer::IntegerVector chiavi = con->getKeys(&relname);
        if( !con->hasErrors() ) {
            unsigned int nChiavi = (unsigned int) chiavi.size();
            for(unsigned int i=0; i<nChiavi; i++) {
                cout << "Colonna chiave:\t" << chiavi[i] << "\t"
                     << con->getColumnName( &relname, chiavi[i] ) << endl;
            }
        } else {
            cout << "Errori: " << con->getErrorMessage() << endl;
        }
    } else {
        cout << "Errori: " << con->getErrorMessage() << endl;
    }
    delete con;
}
void testGetKeys(string host,string dbname,string usr,string pwd, string relname) {
    string connString( "host="+host+" dbname="+dbname+" user="+usr+" password="+pwd );
    testGetKeys(connString, relname);
}

void testGetForeignKeys(string connString, string relname) {
    DBLayer::Connection* con;
    con = DBLayer::createConnection( connString.c_str() );
    con->connect();
    if ( !con->hasErrors() ) {
        cout << "Tabella: " << relname << endl;
        int numColonne = con->getColumnSize( &relname );
        cout << "\tNum. Colonne: " << numColonne << endl;
        for(int c = 1; c<=numColonne; c++) {
            cout << "\t" << c << ") " << con->getColumnName( &relname, c ) << endl;
        }
        DBLayer::IntegerVector chiavi = con->getForeignKeys(&relname);
        if( !con->hasErrors() ) {
            unsigned int nChiavi = (unsigned int) chiavi.size();
            for(unsigned int i=0; i<nChiavi; i++) {
                cout << "Colonna chiave:\t" << chiavi[i] << "\t"
                     << con->getColumnName( &relname, chiavi[i] ) << endl;
            }
        } else {
            cout << "Errori: " << con->getErrorMessage() << endl;
        }
    } else {
        cout << "Errori: " << con->getErrorMessage() << endl;
    }
    delete con;
}
void testGetForeignKeys(string host,string dbname,string usr,string pwd, string relname) {
    string connString( "host="+host+" dbname="+dbname+" user="+usr+" password="+pwd );
    testGetForeignKeys(connString, relname);
}


void testDBMgr(string connString) {
    DBLayer::Connection* con;
    DBMgr* dbmgr;

    con = DBLayer::createConnection( connString.c_str() );
    dbmgr = new DBLayer::DBMgr(con, false);

    if ( dbmgr->connect() ) {
        string nomeTabella = string("test_dblayer");
        string myQuery = string("select * from test_dblayer");
        DBEntityVector* lista = dbmgr->Select( &nomeTabella, &myQuery );

        if ( lista->size()>0 ) {
            cout << "Lista (" << typeid(lista).name() << ") di " << lista->size() << " elementi:" << endl;
            DBEntityVector::iterator theIterator;
            for(theIterator = lista->begin(); theIterator!=lista->end(); theIterator++) {
                string miastringa = (*theIterator)->toString();
                cout << "\t" << miastringa << endl;
            }
            cout << "====" << endl;
        } else {
            cout << "testDBMgr: LISTA VUOTA!!!" << endl;
        }

        cout << "testDBMgr: destroying lista..." << endl;
        DBMgr::Destroy(lista);
        cout << "OK!" << endl;
    } else {
        cout << "Errore: " << dbmgr->getErrorMessage() << endl;
    }
    delete dbmgr;
    delete con;
}
void testDBMgr(string host,string dbname,string usr,string pwd) {
    string connString = string( "host="+host+" dbname="+dbname+" user="+usr+" password="+pwd );
    testDBMgr( connString );
}

void testSearch(string connString) {
    Connection* mycon;
    DBMgr* dbmgr;

    mycon = DBLayer::createConnection( connString.c_str() );
    dbmgr = new DBLayer::DBMgr(mycon, false);

    if ( dbmgr->connect() ) {
        string nomeTabella = string("test_dblayer");
        DBEntity* cerca = new DBEntity( &nomeTabella );

//        string nomeCampo("nome");
//        string valoreStringa("la");
//        cerca.setValue( &nomeCampo, &valoreStringa );

//        string nomeCampoBoolean("abilitato");
//        cerca.setValue( &nomeCampoBoolean, false );

//        string nomeCampoFloat("prezzo");
//        float valoreFloat = 5436.32;
//        cerca.setValue( &nomeCampoFloat, valoreFloat );

//        string nomeCampoInteger("id");
//        long valoreInteger = 1;
//        cerca.setValue( &nomeCampoInteger, valoreInteger );

        string nomeCampoDate("data_creazione");
//	string valoreDate("2006-03-31 20:00:00");
        string valoreDate("2006-04-01 16:15:30");
        cerca->setDateValue( &nomeCampoDate, &valoreDate );

        cout << "testSearch: cerca = " << cerca->toString() << endl;

        string orderBy("nome");
        DBEntityVector* lista = dbmgr->Search(cerca, true, true, &orderBy );

        if ( lista->size()>0 ) {
            cout << "Lista (" << typeid(lista).name() << "):" << endl;
            DBEntityVector::iterator theIterator;
            for(theIterator = lista->begin(); theIterator!=lista->end(); theIterator++) {
                cout << "\t" << (*theIterator)->toString() << endl;
            }
        }

        dbmgr->Destroy(lista);
        delete cerca;
    } else {
        cout << "Errore: " << dbmgr->getErrorMessage() << endl;
    }
    delete dbmgr;
    delete mycon;
}
void testSearch(string host,string dbname,string usr,string pwd) {
    string connString = string( "host="+host+" dbname="+dbname+" user="+usr+" password="+pwd );
    testSearch(connString);
}

void testDBE(string connString) {
    Connection* con;
    DBMgr* dbmgr;
    DBEFactory* dbeFactory;

    con = DBLayer::createConnection( connString.c_str() );
    dbmgr = new DBLayer::DBMgr(con, false);

    dbeFactory = new DBEFactory();
    dbeFactory->registerClass(string("societa"),new DBESocieta());
    dbeFactory->registerClass(string("test_dblayer"),new DBETestDBLayer());

    dbmgr->setDBEFactory(dbeFactory);

    // Stampo la factory
    cout << "testDBE: factory = " << dbeFactory->toString() << endl;

    // Stampo le classi registrate
    DBEntityVector myTypes = dbmgr->getRegisteredTypes();
    for(unsigned int i=0; i<myTypes.size(); i++) {
        cout << "testDBE: registered type " << i
             << " - " << myTypes[i]
             << " - " << myTypes[i]->toString()
             << endl;
    }

    if ( dbmgr->connect() ) {
        string myclazzname("societa");
        string mytypename("DBESocieta");
        DBEntity* cerca = dbmgr->getClazzByTypeName(&mytypename);

        string nomeCampo("ragione_sociale");
        string valoreStringa("a");
        cerca->setValue( &nomeCampo, &valoreStringa );

        //string nomeCampoInteger("cap");
        //long valoreInteger = 20156;
        //cerca->setValue( &nomeCampoInteger, valoreInteger );

        cout << "testDBE: cerca = " << cerca->toString() << endl;

        cout << "testDBE: cerca.getNames() = { ";
        DBLayer::StringVector nomi = cerca->getNames();
        for(unsigned int i=0; i<nomi.size(); i++) {
            cout << nomi[i];
            if( i!=(nomi.size()-1) )
                cout << ", ";
        }
        cout << " }" << endl;

        string orderBy = string("ragione_sociale");
        DBEntityVector* lista = dbmgr->Search(cerca, true, true, &orderBy );
        cout << "testDBE: ricerca completata." << endl;

        if ( lista->size()>0 ) {
            cout << "Lista (" << DBLayer::integer2string((long)lista->size()) << "):" << endl;
            DBEntityVector::iterator theIterator;
            for(theIterator = lista->begin(); theIterator!=lista->end(); theIterator++) {
                cout << "\t" << (*theIterator)->toString() << endl;
            }
        }
        delete cerca;
        dbmgr->Destroy(lista);
    } else {
        cout << "Errore: " << dbmgr->getErrorMessage() << endl;
    }
    delete dbeFactory;
    delete dbmgr;
    delete con;
}
void testDBE(string host,string dbname,string usr,string pwd) {
    string connString = string( "host="+host+" dbname="+dbname+" user="+usr+" password="+pwd );
    testDBE(connString);
}

void testCRUD(string connString) {
    Connection* con;
    DBMgr* dbmgr;
    DBEFactory* dbeFactory;

    int fieldCreati = SchemaNS::getFieldCreati();
    int fieldDistrutti = SchemaNS::getFieldDistrutti();
    int schemiCreati = SchemaNS::getSchemiCreati();
    int schemiDistrutti = SchemaNS::getSchemiDistrutti();
    printf("::testCRUD: Field Creati: %d - Distrutti: %d; Schemi Creati: %d - Distrutti: %d\n",   SchemaNS::getFieldCreati() - fieldCreati, SchemaNS::getFieldDistrutti() - fieldDistrutti, SchemaNS::getSchemiCreati() - schemiCreati, SchemaNS::getSchemiDistrutti() - schemiDistrutti );

    con = DBLayer::createConnection( connString.c_str() );

    dbmgr = new DBLayer::DBMgr(con, false);

    dbeFactory = new DBEFactory();
    dbeFactory->registerClass(string("societa"),new DBESocieta());
    dbeFactory->registerClass(string("test_dblayer"),new DBETestDBLayer());

    dbmgr->setDBEFactory(dbeFactory);
    cout << "testCRUD: Creata DBEFactory." << endl;
    printf("::testCRUD: Field Creati: %d - Distrutti: %d; Schemi Creati: %d - Distrutti: %d\n",   SchemaNS::getFieldCreati() - fieldCreati, SchemaNS::getFieldDistrutti() - fieldDistrutti, SchemaNS::getSchemiCreati() - schemiCreati, SchemaNS::getSchemiDistrutti() - schemiDistrutti );

    if ( dbmgr->connect() ) {
        string myclazzname("societa");
        DBEntity* nuova = dbmgr->getClazz(&myclazzname);
        string nomeCampo("ragione_sociale");
        string valoreStringa("Nuova Societa S.r.l.");
        nuova->setValue( &nomeCampo, &valoreStringa );
        nomeCampo="data_creazione";
        valoreStringa="1970-02-01 00:00:00";
        nuova->setDateValue(&nomeCampo,&valoreStringa);

        cout << "testCRUD: da inserire " << nuova->toString() << endl;
        printf("::testCRUD: Field Creati: %d - Distrutti: %d; Schemi Creati: %d - Distrutti: %d\n",   SchemaNS::getFieldCreati() - fieldCreati, SchemaNS::getFieldDistrutti() - fieldDistrutti, SchemaNS::getSchemiCreati() - schemiCreati, SchemaNS::getSchemiDistrutti() - schemiDistrutti );

        // Insert
        cout << endl;
        cout << "testCRUD: provo ad inserire una nuova dbe " << endl;
        nuova = dbmgr->Insert(nuova);
        cout << "testCRUD: inserita nuova dbe " << nuova->toString() << endl;
        printf("::testCRUD: Field Creati: %d - Distrutti: %d; Schemi Creati: %d - Distrutti: %d\n",   SchemaNS::getFieldCreati() - fieldCreati, SchemaNS::getFieldDistrutti() - fieldDistrutti, SchemaNS::getSchemiCreati() - schemiCreati, SchemaNS::getSchemiDistrutti() - schemiDistrutti );

        DBEntity* cerca = dbmgr->getClazz(&myclazzname);
        string nomeCampoId("id");
        DBField* idField = (DBField*) nuova->getField(&nomeCampoId);
        cerca->setValue(&nomeCampoId, idField!=0 ? idField->getIntegerValue() : -1 );
        string orderBy = string("id");
        cout << "testCRUD: cerca=" << cerca->toString() << endl;
        printf("::testCRUD: Field Creati: %d - Distrutti: %d; Schemi Creati: %d - Distrutti: %d\n",   SchemaNS::getFieldCreati() - fieldCreati, SchemaNS::getFieldDistrutti() - fieldDistrutti, SchemaNS::getSchemiCreati() - schemiCreati, SchemaNS::getSchemiDistrutti() - schemiDistrutti );

        cout << endl;
        DBEntityVector* lista = dbmgr->Search(cerca, true, true, &orderBy );
        if ( lista->size()>0 ) {
            cout << "Lista (" << DBLayer::integer2string((long)lista->size()) << "):" << endl;
            for(DBEntityVector::iterator theIterator = lista->begin(); theIterator!=lista->end(); theIterator++) {
                cout << "- " << (*theIterator)->toString() << endl;
            }
        } else {
            cout << "testCRUD: LISTA VUOTA!!!" << endl;
        }
        dbmgr->Destroy(lista);
        printf("::testCRUD: Field Creati: %d - Distrutti: %d; Schemi Creati: %d - Distrutti: %d\n",   SchemaNS::getFieldCreati() - fieldCreati, SchemaNS::getFieldDistrutti() - fieldDistrutti, SchemaNS::getSchemiCreati() - schemiCreati, SchemaNS::getSchemiDistrutti() - schemiDistrutti );

        // Update
        string valoreStringa2("Nuova Societa L\'Attico S.r.l.");
        nuova->setValue( &nomeCampo, &valoreStringa2 );
        string nomeCampoCap("cap");
        nuova->setValue( &nomeCampoCap, 60015L );

        cout << endl;
        nuova = dbmgr->Update(nuova);
        cout << "testCRUD: modificata nuova dbe " << nuova->toString() << endl;
        printf("::testCRUD: Field Creati: %d - Distrutti: %d; Schemi Creati: %d - Distrutti: %d\n",   SchemaNS::getFieldCreati() - fieldCreati, SchemaNS::getFieldDistrutti() - fieldDistrutti, SchemaNS::getSchemiCreati() - schemiCreati, SchemaNS::getSchemiDistrutti() - schemiDistrutti );

        cout << endl;
        lista = dbmgr->Search(cerca, true, true, &orderBy );
        if ( lista->size()>0 ) {
            cout << "Lista (" << DBLayer::integer2string((long)lista->size()) << "):" << endl;
            for(DBEntityVector::iterator theIterator = lista->begin(); theIterator!=lista->end(); theIterator++) {
                cout << "- " << (*theIterator)->toString() << endl;
            }
        } else {
            cout << "testCRUD: LISTA VUOTA!!!" << endl;
        }
        dbmgr->Destroy(lista);
        printf("::testCRUD: Field Creati: %d - Distrutti: %d; Schemi Creati: %d - Distrutti: %d\n",   SchemaNS::getFieldCreati() - fieldCreati, SchemaNS::getFieldDistrutti() - fieldDistrutti, SchemaNS::getSchemiCreati() - schemiCreati, SchemaNS::getSchemiDistrutti() - schemiDistrutti );

        // Copy
        cout << endl;
        DBEntity* copia = dbmgr->Copy( nuova );
        if(copia!=0) {
            cout << "testCRUD: creata copia dbe " << copia->toString() << endl;
        } else {
            cout << "testCRUD: FALLITA copia" << endl;
        }
        printf("::testCRUD: Field Creati: %d - Distrutti: %d; Schemi Creati: %d - Distrutti: %d\n",   SchemaNS::getFieldCreati() - fieldCreati, SchemaNS::getFieldDistrutti() - fieldDistrutti, SchemaNS::getSchemiCreati() - schemiCreati, SchemaNS::getSchemiDistrutti() - schemiDistrutti );

        cout << endl;
        DBEntity* cercaCopia = dbmgr->getClazz(&myclazzname);
        string nomeCampoRagioneSociale("ragione_sociale");
        string valoreCampoRagioneSociale("Societa");
        cercaCopia->setValue( &nomeCampoRagioneSociale, &valoreCampoRagioneSociale );
        lista = dbmgr->Search(cercaCopia, true, true, &orderBy );
        if ( lista->size()>0 ) {
            cout << "Lista (" << DBLayer::integer2string((long)lista->size()) << "):" << endl;
            for(DBEntityVector::iterator theIterator = lista->begin(); theIterator!=lista->end(); theIterator++) {
                cout << "- " << (*theIterator)->toString() << endl;
            }
        } else {
            cout << "testCRUD: LISTA VUOTA!!!" << endl;
        }
        dbmgr->Destroy(lista);
        printf("::testCRUD: Field Creati: %d - Distrutti: %d; Schemi Creati: %d - Distrutti: %d\n",   SchemaNS::getFieldCreati() - fieldCreati, SchemaNS::getFieldDistrutti() - fieldDistrutti, SchemaNS::getSchemiCreati() - schemiCreati, SchemaNS::getSchemiDistrutti() - schemiDistrutti );


        // Delete
        cout << endl;
        nuova = dbmgr->Delete(nuova);
        cout << "testCRUD: cancellata nuova dbe " << nuova->toString() << endl;
        cout << endl;
        if(copia!=0) {
            copia = dbmgr->Delete(copia);
            cout << "testCRUD: cancellata copia dbe " << copia->toString() << endl;
        }
        printf("::testCRUD: Field Creati: %d - Distrutti: %d; Schemi Creati: %d - Distrutti: %d\n",   SchemaNS::getFieldCreati() - fieldCreati, SchemaNS::getFieldDistrutti() - fieldDistrutti, SchemaNS::getSchemiCreati() - schemiCreati, SchemaNS::getSchemiDistrutti() - schemiDistrutti );

        cout << endl;
        lista = dbmgr->Search(cercaCopia, true, true, &orderBy );
        if ( lista->size()>0 ) {
            cout << "Lista (" << DBLayer::integer2string((long)lista->size()) << "):" << endl;
            for(DBEntityVector::iterator theIterator = lista->begin(); theIterator!=lista->end(); theIterator++) {
                cout << "- " << (*theIterator)->toString() << endl;
            }
        } else {
            cout << "testCRUD: LISTA VUOTA!!!" << endl;
        }
        dbmgr->Destroy(lista);
        printf("::testCRUD: Field Creati: %d - Distrutti: %d; Schemi Creati: %d - Distrutti: %d\n",   SchemaNS::getFieldCreati() - fieldCreati, SchemaNS::getFieldDistrutti() - fieldDistrutti, SchemaNS::getSchemiCreati() - schemiCreati, SchemaNS::getSchemiDistrutti() - schemiDistrutti );

        delete nuova;
        printf("::testCRUD: Field Creati: %d - Distrutti: %d; Schemi Creati: %d - Distrutti: %d\n",   SchemaNS::getFieldCreati() - fieldCreati, SchemaNS::getFieldDistrutti() - fieldDistrutti, SchemaNS::getSchemiCreati() - schemiCreati, SchemaNS::getSchemiDistrutti() - schemiDistrutti );
        delete copia;
        printf("::testCRUD: Field Creati: %d - Distrutti: %d; Schemi Creati: %d - Distrutti: %d\n",   SchemaNS::getFieldCreati() - fieldCreati, SchemaNS::getFieldDistrutti() - fieldDistrutti, SchemaNS::getSchemiCreati() - schemiCreati, SchemaNS::getSchemiDistrutti() - schemiDistrutti );
        delete cerca;
        printf("::testCRUD: Field Creati: %d - Distrutti: %d; Schemi Creati: %d - Distrutti: %d\n",   SchemaNS::getFieldCreati() - fieldCreati, SchemaNS::getFieldDistrutti() - fieldDistrutti, SchemaNS::getSchemiCreati() - schemiCreati, SchemaNS::getSchemiDistrutti() - schemiDistrutti );
        delete cercaCopia;
    } else {
        cout << "testCRUD: ERRORE " << dbmgr->getErrorMessage() << endl;
    }
    printf("::testCRUD: Field Creati: %d - Distrutti: %d; Schemi Creati: %d - Distrutti: %d\n",   SchemaNS::getFieldCreati() - fieldCreati, SchemaNS::getFieldDistrutti() - fieldDistrutti, SchemaNS::getSchemiCreati() - schemiCreati, SchemaNS::getSchemiDistrutti() - schemiDistrutti );
    delete dbmgr;
    delete dbeFactory;
    printf("::testCRUD: Field Creati: %d - Distrutti: %d; Schemi Creati: %d - Distrutti: %d\n",   SchemaNS::getFieldCreati() - fieldCreati, SchemaNS::getFieldDistrutti() - fieldDistrutti, SchemaNS::getSchemiCreati() - schemiCreati, SchemaNS::getSchemiDistrutti() - schemiDistrutti );
    delete con;
}
void testCRUD(string host,string dbname,string usr,string pwd) {
    string connString = string( "host="+host+" dbname="+dbname+" user="+usr+" password="+pwd );
    testCRUD( connString );
}




int main(int argc, char *argv[]) {
    string host,dbname,usr,pwd;
    string connString("dblayer:sqlite:./examples/test.db");
    //string connString("dblayer:mysql:host=localhost;dbname=rproject;user=root;password=;");
    //string connString("dblayer:pg:host=localhost dbname=roberto user=roberto password=roberto");
    //cout << "Content-type: text/html" << endl << endl;

    if(argc!=2 && argc!=5) {
        cerr << "Usage: " << argv[0] << " connect-string" << endl
               << "or     " << argv[0] << " dsn username password" << endl;
        host = string("localhost");
        dbname = string("roberto");
        usr = string("roberto");
        pwd = string("echoestrade");
    } else if (argc==5) {
        host = string( argv[1] );
        dbname = string( argv[2] );
        usr = string( argv[3] );
        pwd = string( argv[4] );
    } else {
        connString = string( argv[1] );
    }
    //cout << "connString: " << connString << endl;

    cout << "---------------->>  testDBConnection" << endl;
    if( argc==5 ) {
        testDBConnection( host, dbname, usr, pwd );
    } else {
        testDBConnection( connString );
    }
    printf("Field Creati: %d\n",   SchemaNS::getFieldCreati() );
    printf("Field Distrutti: %d\n",SchemaNS::getFieldDistrutti() );
    printf("Schemi Creati: %d\n",   SchemaNS::getSchemiCreati() );
    printf("Schemi Distrutti: %d\n",SchemaNS::getSchemiDistrutti() );

    cout << "---------------->>  testGetKeys" << endl;
    if(argc==5) {
        testGetKeys( host, dbname, usr, pwd, string("societa") );
    } else {
        testGetKeys( connString, string("societa") );
    }

    testDateField();

    cout << "---------------->>  testDBMgr" << endl;
    if ( argc==5 ) {
        testDBMgr( host, dbname, usr, pwd );
    } else {
        testDBMgr( connString );
    }
    printf("Field Creati: %d\n",   SchemaNS::getFieldCreati() );
    printf("Field Distrutti: %d\n",SchemaNS::getFieldDistrutti() );
    printf("Schemi Creati: %d\n",   SchemaNS::getSchemiCreati() );
    printf("Schemi Distrutti: %d\n",SchemaNS::getSchemiDistrutti() );

    cout << "---------------->>  testSearch" << endl;
    if(argc==5) {
        testSearch( host, dbname, usr, pwd );
    } else {
        testSearch( connString );
    }
    printf("Field Creati: %d\n",   SchemaNS::getFieldCreati() );
    printf("Field Distrutti: %d\n",SchemaNS::getFieldDistrutti() );
    printf("Schemi Creati: %d\n",   SchemaNS::getSchemiCreati() );
    printf("Schemi Distrutti: %d\n",SchemaNS::getSchemiDistrutti() );

    cout << "---------------->>  testDBE" << endl;
    if(argc==5) {
        testDBE( host, dbname, usr, pwd );
    } else {
        testDBE( connString );
    }
    printf("Field Creati: %d\n",   SchemaNS::getFieldCreati() );
    printf("Field Distrutti: %d\n",SchemaNS::getFieldDistrutti() );
    printf("Schemi Creati: %d\n",   SchemaNS::getSchemiCreati() );
    printf("Schemi Distrutti: %d\n",SchemaNS::getSchemiDistrutti() );

    cout << "---------------->>  testCRUD" << endl;
    if(argc==5) {
        testCRUD( host, dbname, usr, pwd );
    } else {
        testCRUD( connString );
    }

    printf("Field Creati: %d\n",   SchemaNS::getFieldCreati() );
    printf("Field Distrutti: %d\n",SchemaNS::getFieldDistrutti() );
    printf("Schemi Creati: %d\n",   SchemaNS::getSchemiCreati() );
    printf("Schemi Distrutti: %d\n",SchemaNS::getSchemiDistrutti() );

    cout << "---------------->>  testGetKeys" << endl;
    if( argc==5 ) {
        testGetKeys( host, dbname, usr, pwd, "test_dblayer" );
        testGetKeys( host, dbname, usr, pwd, "societa" );
    } else {
        testGetKeys( connString, "test_dblayer" );
        testGetKeys( connString, "societa" );
    }

    cout << "---------------->>  testGetForeignKeys" << endl;
    if( argc==5 ) {
        testGetForeignKeys( host, dbname, usr, pwd, "test_dblayer" );
        testGetForeignKeys( host, dbname, usr, pwd, "societa" );
    } else {
        testGetForeignKeys( connString, "test_dblayer" );
        testGetForeignKeys( connString, "societa" );
    }

    return EXIT_SUCCESS;
}
