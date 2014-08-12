/** *************************************************************************
**	main.cpp  v0.0.1 - 2006.04.03
**	-----------------------------
**
**	Author:		Roberto Rocco Angeloni.
**	E-mail:			roberto@roccoangeloni.it
**	Copyright:	(c) 2011-2014 by Roberto Rocco Angeloni.
**	Comment:
**		dblayer:odbc:roberto
**		dblayer:odbc:test_dblayer
**		"dblayer:pg:host=localhost dbname=roberto user=roberto password=roberto"
**		dblayer:sqlite:F:\Roberto\CVS_Workspace\CPP\db_layer\db_layer2\test.db
**		dblayer:sqlite:./test.db
**		dblayer:xmlrpc:http://localhost/~roberto/rproject/xmlrpc_server.php
**		dblayer:xmlrpc:http://www.roccoangeloni.it/rproject/xmlrpc_server.php
**		"dblayer:mysql:host=localhost;dbname=roberto;user=root;password=;"
**      dblayer:qxmlrpc:http://127.0.0.1/~roberto/rp/xmlrpc_server.php
**
**
** cmake . ; make && ./src/dblayer_test dblayer:sqlite:./test.db
**
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

#include "dblayer/authschema.h"
using namespace AuthSchema;

#include <config.h>

#ifdef USE_QXMLRPC
#include <QApplication>
#include "qdblayer/qxmlrpcconnection.h"
#include <QDebug>
#endif

#if defined(WIN32)
# define snprintf _snprintf
#endif

#include <iostream>
#include <stdlib.h>
#include <typeinfo>

using namespace std;

void testDBConnection(string& connString, string& loginUser, string& loginPwd) {
    DBLayer::Connection* con;
    DBLayer::ResultSet* res;

    con = DBLayer::createConnection( connString.c_str() );
    con->connect();

    if(loginUser.length()>0 && loginPwd.length()>0) {
        con->login(loginUser,loginPwd);
    }

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
            cout << "res.status: " << res->getStatus() << "." << endl;
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
            cout << "Errors: " << con->getErrorMessage() << endl;
        }
        delete res;
    } else {
        cout << "(testDBConnection) Errors: " << con->getErrorMessage() << endl;
    }
    //con->disconnect();
    delete con;
}
void testDBConnection(string host,string dbname,string usr,string pwd, string& loginUser, string& loginPwd) {
    string connString = string( "host="+host+" dbname="+dbname+" user="+usr+" password="+pwd );
    testDBConnection( connString, loginUser, loginPwd );
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

void testGetKeys(string connString, string& loginUser, string& loginPwd, string relname) {
    DBLayer::Connection* con;

    con = DBLayer::createConnection( connString.c_str() );
    con->connect();

    if(loginUser.length()>0 && loginPwd.length()>0) {
        con->login(loginUser,loginPwd);
    }

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
            cout << "Errors: " << con->getErrorMessage() << endl;
        }
    } else {
        cout << "Errors: " << con->getErrorMessage() << endl;
    }
    delete con;
}
void testGetKeys(string host,string dbname,string usr,string pwd, string& loginUser, string& loginPwd, string relname) {
    string connString( "host="+host+" dbname="+dbname+" user="+usr+" password="+pwd );
    testGetKeys(connString, loginUser, loginPwd, relname);
}

void testGetForeignKeys(string connString, string& loginUser, string& loginPwd, string relname) {
    DBLayer::Connection* con;
    con = DBLayer::createConnection( connString.c_str() );
    con->connect();

    if(loginUser.length()>0 && loginPwd.length()>0) {
        con->login(loginUser,loginPwd);
    }

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
            cout << "Errors: " << con->getErrorMessage() << endl;
        }
    } else {
        cout << "Errors: " << con->getErrorMessage() << endl;
    }
    delete con;
}
void testGetForeignKeys(string host,string dbname,string usr,string pwd, string& loginUser, string& loginPwd, string relname) {
    string connString( "host="+host+" dbname="+dbname+" user="+usr+" password="+pwd );
    testGetForeignKeys(connString, loginUser, loginPwd, relname);
}

void testDBMgr(string connString, string& loginUser, string& loginPwd) {
    DBLayer::Connection* con;
    DBMgr* dbmgr;

    con = DBLayer::createConnection( connString.c_str() );
    dbmgr = new DBLayer::DBMgr(con, true);

    DBEFactory dbeFactory(false);
    dbmgr->setDBEFactory(&dbeFactory);
    AuthSchema::registerClasses(&dbeFactory);
    AuthSchema::checkDB(*dbmgr);

    if(dbmgr->connect()) {

        if(loginUser.length()>0 && loginPwd.length()>0) {
            dbmgr->login(loginUser,loginPwd);
        }

        if(dbmgr->isLoggedIn()) {
            string nomeTabella = string("users"); //test_dblayer");
            string myQuery = string("select * from rra_users"); //test_dblaye
            DBEntityVector* lista = dbmgr->Select( &nomeTabella, &myQuery );

            if(lista->size()>0) {
                cout << "Lista (" << typeid(lista).name() << ") di " << lista->size() << " elementi:" << endl;
                for(const auto& elem : (*lista)) {
                    cout << "- " << elem->toString("\n") << endl;
                }
                cout << "====" << endl;
            } else {
                cout << "testDBMgr: LISTA VUOTA!!!" << endl;
            }

            cout << "testDBMgr: destroying lista..." << endl;
            DBMgr::Destroy(lista);
            cout << "OK!" << endl;
        } else {
            cout << "Login Error: " << dbmgr->getErrorMessage() << "." << endl;
        }
    } else {
        cout << "Connection Error: " << dbmgr->getErrorMessage() << endl;
    }
    delete dbmgr;
    delete con;
}
void testDBMgr(string host,string dbname,string usr,string pwd, string& loginUser, string& loginPwd) {
    string connString = string( "host="+host+" dbname="+dbname+" user="+usr+" password="+pwd );
    testDBMgr( connString, loginUser, loginPwd );
}

void testSearch(string connString, string& loginUser, string& loginPwd) {
    Connection* mycon;
    DBMgr* dbmgr;

    mycon = DBLayer::createConnection( connString.c_str() );
    dbmgr = new DBLayer::DBMgr(mycon, true);

    DBEFactory dbeFactory(false);
    AuthSchema::registerClasses(&dbeFactory);
    MySchema::registerClasses(&dbeFactory);
    dbmgr->setDBEFactory(&dbeFactory);

    if ( dbmgr->connect() ) {
        DBETestDBLayer* cerca = new DBETestDBLayer();

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
            for(const auto& elem : (*lista)) {
                cout << "- " << elem->toString() << endl;
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
void testSearch(string host,string dbname,string usr,string pwd, string& loginUser, string& loginPwd) {
    string connString = string( "host="+host+" dbname="+dbname+" user="+usr+" password="+pwd );
    testSearch(connString, loginUser, loginPwd);
}

void testDBE(string connString, string& loginUser, string& loginPwd) {
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
            for(const auto& elem : (*lista)) {
                cout << "- " << elem->toString() << endl;
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
void testDBE(string host,string dbname,string usr,string pwd, string& loginUser, string& loginPwd) {
    string connString = string( "host="+host+" dbname="+dbname+" user="+usr+" password="+pwd );
    testDBE(connString, loginUser, loginPwd);
}

void testCRUD(string connString, string& loginUser, string& loginPwd) {
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
            for(const auto& elem : (*lista)) {
                cout << "- " << elem->toString() << endl;
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
            for(const auto& elem : (*lista)) {
                cout << "- " << elem->toString() << endl;
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
            for(const auto& elem : (*lista)) {
                cout << "- " << elem->toString() << endl;
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
            for(const auto& elem : (*lista)) {
                cout << "- " << elem->toString() << endl;
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
void testCRUD(string host,string dbname,string usr,string pwd, string& loginUser, string& loginPwd) {
    string connString = string( "host="+host+" dbname="+dbname+" user="+usr+" password="+pwd );
    testCRUD( connString, loginUser, loginPwd );
}

void testGetColumnsForTable(string connString, string& loginUser, string& loginPwd, string relname) {
    Connection* con;
    DBMgr* dbmgr;
    DBEFactory* dbeFactory;

    int fieldCreati = SchemaNS::getFieldCreati();
    int fieldDistrutti = SchemaNS::getFieldDistrutti();
    int schemiCreati = SchemaNS::getSchemiCreati();
    int schemiDistrutti = SchemaNS::getSchemiDistrutti();
    printf("::testGetColumnsForTable: Field Creati: %d - Distrutti: %d; Schemi Creati: %d - Distrutti: %d\n",   SchemaNS::getFieldCreati() - fieldCreati, SchemaNS::getFieldDistrutti() - fieldDistrutti, SchemaNS::getSchemiCreati() - schemiCreati, SchemaNS::getSchemiDistrutti() - schemiDistrutti );

    con = DBLayer::createConnection( connString.c_str() );

    dbmgr = new DBLayer::DBMgr(con, false);

    dbeFactory = new DBEFactory();
    dbeFactory->registerClass("societa",new DBESocieta());
    dbeFactory->registerClass("test_dblayer",new DBETestDBLayer());

    dbmgr->setDBEFactory(dbeFactory);
    cout << "testGetColumnsForTable: Creata DBEFactory." << endl;
    printf("::testGetColumnsForTable: Field Creati: %d - Distrutti: %d; Schemi Creati: %d - Distrutti: %d\n",   SchemaNS::getFieldCreati() - fieldCreati, SchemaNS::getFieldDistrutti() - fieldDistrutti, SchemaNS::getSchemiCreati() - schemiCreati, SchemaNS::getSchemiDistrutti() - schemiDistrutti );

    if ( dbmgr->connect() ) {
        // Insert
        cout << endl;
        cout << "testGetColumnsForTable: recupero le definizioni per la tabella " << relname << endl;
        ColumnDefinitions cols = dbmgr->getColumnsForTable(relname);

        for(const auto& elem : cols) {
            cout << elem.first << endl;
            string glue(",");
            cout << "  " << joinString( (DBLayer::StringVector*) &(elem.second),&glue) << endl;
        }
    } else {
        cout << "testGetColumnsForTable: ERRORE " << dbmgr->getErrorMessage() << endl;
    }
    printf("::testGetColumnsForTable: Field Creati: %d - Distrutti: %d; Schemi Creati: %d - Distrutti: %d\n",   SchemaNS::getFieldCreati() - fieldCreati, SchemaNS::getFieldDistrutti() - fieldDistrutti, SchemaNS::getSchemiCreati() - schemiCreati, SchemaNS::getSchemiDistrutti() - schemiDistrutti );
    delete dbmgr;
    delete dbeFactory;
    printf("::testGetColumnsForTable: Field Creati: %d - Distrutti: %d; Schemi Creati: %d - Distrutti: %d\n",   SchemaNS::getFieldCreati() - fieldCreati, SchemaNS::getFieldDistrutti() - fieldDistrutti, SchemaNS::getSchemiCreati() - schemiCreati, SchemaNS::getSchemiDistrutti() - schemiDistrutti );
    delete con;
}
void testGetColumnsForTable(string host,string dbname,string usr,string pwd, string& loginUser, string& loginPwd, string relname) {
    string connString = string( "host="+host+" dbname="+dbname+" user="+usr+" password="+pwd );
    testGetColumnsForTable( connString, loginUser, loginPwd, relname );
}



void testThis() {
    DBEntity dbe;
    cout << dbe.toString("\n") << endl;

    DBEDBVersion dbversion;
    cout << dbversion.toString("\n") << endl;

    DBEUserGroup dbeug;
    cout << dbeug.toString("\n") << endl;

    DBEGroup dbeg;
    cout << dbeg.toString("\n") << endl;

    DBELog dbelog;
    cout << dbelog.toString("\n") << endl;

    DBEUser dbeu;
    cout << dbeu.toString("\n") << endl;
}



int main(int argc, char *argv[]) {
    string host,dbname,usr,pwd,login_user,login_password;
    //string connString("dblayer:sqlite:./examples/test.db");
    string connString("dblayer:qxmlrpc:http://127.0.0.1/~roberto/rp/xmlrpc_server.php");
    //string connString("dblayer:mysql:host=localhost;dbname=rproject;user=root;password=;");
    //string connString("dblayer:pg:host=localhost dbname=roberto user=roberto password=roberto");
    //cout << "Content-type: text/html" << endl << endl;


//    testThis();
//    if(false) return 0;

    if(argc!=2 && argc!=4 && argc!=5) {
        cerr << "Usage: " << argv[0] << " connect-string" << endl
               << "or     " << argv[0] << " connect-string login_user login_password" << endl
               << "or     " << argv[0] << " dsn username password" << endl;
        host = string("localhost");
        dbname = string("roberto");
        usr = string("roberto");
        pwd = string("echoestrade");
        return 1;
    } else if (argc==5) {
        host = string( argv[1] );
        dbname = string( argv[2] );
        usr = string( argv[3] );
        pwd = string( argv[4] );
    } else if (argc==4) {
        connString = string( argv[1] );
        login_user = string( argv[2] );
        login_password = string( argv[3] );
    } else if (argc==2) {
        connString = string( argv[1] );
    }
    cout << "connString: " << connString << endl;

#ifdef USE_QXMLRPC
    QApplication a(argc, argv);
    QXmlrpcConnection::registerClass();
#endif

    cout << "---------------->>  testDBConnection" << endl;
    if( argc==5 ) {
        testDBConnection( host, dbname, usr, pwd, login_user, login_password );
    } else {
        testDBConnection( connString, login_user, login_password );
    }
    printf("Field Creati: %d\n",   SchemaNS::getFieldCreati() );
    printf("Field Distrutti: %d\n",SchemaNS::getFieldDistrutti() );
    printf("Schemi Creati: %d\n",   SchemaNS::getSchemiCreati() );
    printf("Schemi Distrutti: %d\n",SchemaNS::getSchemiDistrutti() );

    cout << "---------------->>  testGetKeys" << endl;
    if(argc==5) {
        testGetKeys( host, dbname, usr, pwd, login_user, login_password, string("societa") );
    } else {
        testGetKeys( connString, login_user, login_password, string("societa") );
    }

    testDateField();

    cout << "---------------->>  testDBMgr: start." << endl;
    if ( argc==5 ) {
        testDBMgr( host, dbname, usr, pwd, login_user, login_password );
    } else {
        testDBMgr( connString, login_user, login_password );
    }
    printf("\n");
    printf("Field Creati: %d\n",   SchemaNS::getFieldCreati() );
    printf("Field Distrutti: %d\n",SchemaNS::getFieldDistrutti() );
    printf("Schemi Creati: %d\n",   SchemaNS::getSchemiCreati() );
    printf("Schemi Distrutti: %d\n",SchemaNS::getSchemiDistrutti() );
    cout << "---------------->>  testDBMgr: end." << endl;
    cout << endl;

    cout << "---------------->>  testSearch" << endl;
    if(argc==5) {
        testSearch( host, dbname, usr, pwd, login_user, login_password );
    } else {
        testSearch( connString, login_user, login_password );
    }
    printf("Field Creati: %d\n",   SchemaNS::getFieldCreati() );
    printf("Field Distrutti: %d\n",SchemaNS::getFieldDistrutti() );
    printf("Schemi Creati: %d\n",   SchemaNS::getSchemiCreati() );
    printf("Schemi Distrutti: %d\n",SchemaNS::getSchemiDistrutti() );

    return 0;

    cout << "---------------->>  testDBE" << endl;
    if(argc==5) {
        testDBE( host, dbname, usr, pwd, login_user, login_password );
    } else {
        testDBE( connString, login_user, login_password );
    }
    printf("Field Creati: %d\n",   SchemaNS::getFieldCreati() );
    printf("Field Distrutti: %d\n",SchemaNS::getFieldDistrutti() );
    printf("Schemi Creati: %d\n",   SchemaNS::getSchemiCreati() );
    printf("Schemi Distrutti: %d\n",SchemaNS::getSchemiDistrutti() );

    cout << "---------------->>  testCRUD" << endl;
    if(argc==5) {
        testCRUD( host, dbname, usr, pwd, login_user, login_password );
    } else {
        testCRUD( connString, login_user, login_password );
    }

    printf("Field Creati: %d\n",   SchemaNS::getFieldCreati() );
    printf("Field Distrutti: %d\n",SchemaNS::getFieldDistrutti() );
    printf("Schemi Creati: %d\n",   SchemaNS::getSchemiCreati() );
    printf("Schemi Distrutti: %d\n",SchemaNS::getSchemiDistrutti() );

    cout << "---------------->>  testGetKeys" << endl;
    if( argc==5 ) {
        testGetKeys( host, dbname, usr, pwd, login_user, login_password, "test_dblayer" );
        testGetKeys( host, dbname, usr, pwd, login_user, login_password, "societa" );
    } else {
        testGetKeys( connString, login_user, login_password, "test_dblayer" );
        testGetKeys( connString, login_user, login_password, "societa" );
    }

    cout << "---------------->>  testGetForeignKeys" << endl;
    if( argc==5 ) {
        testGetForeignKeys( host, dbname, usr, pwd, login_user, login_password, "test_dblayer" );
        testGetForeignKeys( host, dbname, usr, pwd, login_user, login_password, "societa" );
    } else {
        testGetForeignKeys( connString, login_user, login_password, "test_dblayer" );
        testGetForeignKeys( connString, login_user, login_password, "societa" );
    }

    cout << "---------------->>  testGetColumnsForTable" << endl;
    if( argc==5 ) {
        testGetColumnsForTable( host, dbname, usr, pwd, login_user, login_password, "test_dblayer" );
        testGetColumnsForTable( host, dbname, usr, pwd, login_user, login_password, "societa" );
        testGetColumnsForTable( host, dbname, usr, pwd, login_user, login_password, "rra_users" );
    } else {
        testGetColumnsForTable( connString, login_user, login_password, "test_dblayer" );
        testGetColumnsForTable( connString, login_user, login_password, "societa" );
        testGetColumnsForTable( connString, login_user, login_password, "rra_users" );
        //testGetColumnsForTable( connString, login_user, login_password, "rra_people" );
    }

    return EXIT_SUCCESS;
}
