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
using namespace TestSchema;

#include "dblayer/authschema.h"
using namespace AuthSchema;

#include <config.h>

#ifdef USE_QXMLRPC
#include <QApplication>
#include "qdblayer/qsqliteconnection.h"
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

void testDateField() {
    DBLayer::DateField dateField = DBLayer::DateField( "cippa", "1970-02-01 00:00:00" );
    cout << "string: " << dateField.toString() << endl;
    cout << "seconds: " << dateField.to_seconds() << endl;
    cout << "days: " << dateField.to_days() << endl;

    dateField.setValue( dateField.to_seconds() );
    cout << "seconds2date: " << dateField.toString() << endl;

    dateField.setValue( "2038-01-19 00:00:00" );
    cout << "date: " << dateField.toString() << endl;
    dateField.setValue( dateField.to_seconds() );
    cout << "seconds2date: " << dateField.toString() << endl;
}

void testDBMgr(string connString, string& loginUser, string& loginPwd) {
    DBLayer::Connection* con;
    DBMgr* dbmgr;

    con = DBLayer::createConnection( connString.c_str() );
    //con->setVerbose(true);
    dbmgr = new DBLayer::DBMgr(con, false);

    DBEFactory dbeFactory(false);
    dbmgr->setDBEFactory(&dbeFactory);
    //dbmgr->setSchema("rra");
    AuthSchema::registerClasses(&dbeFactory);
    AuthSchema::checkDB(*dbmgr,false);
    TestSchema::registerClasses(&dbeFactory);
    TestSchema::checkDB(*dbmgr,false);

    if(dbmgr->connect()) {

        if(loginUser.length()>0 && loginPwd.length()>0) {
            dbmgr->login(loginUser,loginPwd);
        }

        if(dbmgr->isLoggedIn()) {
            DBEntityVector* lista = dbmgr->Select("test_dblayer","select * from test_test_dblayer");

            if(lista->size()>0) {
                cout << "List (" << typeid(lista).name() << ") of " << lista->size() << " elements:" << endl;
                for(const auto& elem : (*lista)) {
                    cout << "- " << elem->toString() << endl;
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
    dbmgr = new DBLayer::DBMgr(mycon, false);

    DBEFactory dbeFactory(false);
    dbmgr->setDBEFactory(&dbeFactory);
    //dbmgr->setSchema("rra");
    AuthSchema::registerClasses(&dbeFactory);
    AuthSchema::checkDB(*dbmgr);
    TestSchema::registerClasses(&dbeFactory);
    TestSchema::checkDB(*dbmgr);

    if(dbmgr->connect()) {
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

        cerca->setDateValue("data_creazione","2006-04-01 16:15:30");

        cout << "testSearch: cerca = " << cerca->toString() << endl;

        DBEntityVector* lista = dbmgr->Search(cerca,true,true,"nome");

        if ( lista->size()>0 ) {
            cout << "List (" << typeid(lista).name() << "):" << endl;
            for(const auto& elem : (*lista)) {
                cout << "- " << elem->toString() << endl;
            }
        }

        dbmgr->Destroy(lista);
        delete cerca;
    } else {
        cout << "Error: " << dbmgr->getErrorMessage() << endl;
    }
    delete dbmgr;
    delete mycon;
}
void testSearch(string host,string dbname,string usr,string pwd, string& loginUser, string& loginPwd) {
    string connString = string( "host="+host+" dbname="+dbname+" user="+usr+" password="+pwd );
    testSearch(connString, loginUser, loginPwd);
}

void testDBE(string connString, string& loginUser, string& loginPwd) {
    Connection* mycon;
    DBMgr* dbmgr;

    mycon = DBLayer::createConnection( connString.c_str() );
    dbmgr = new DBLayer::DBMgr(mycon, false);

    DBEFactory dbeFactory(false);
    dbmgr->setDBEFactory(&dbeFactory);
    //dbmgr->setSchema("rra");
    AuthSchema::registerClasses(&dbeFactory);
    AuthSchema::checkDB(*dbmgr);
    TestSchema::registerClasses(&dbeFactory);
    TestSchema::checkDB(*dbmgr);

    // Stampo la factory
    cout << "testDBE: factory = " << dbeFactory.toString() << endl;

    // Stampo le classi registrate
    DBEntityVector myTypes = dbmgr->getRegisteredTypes();
    for(unsigned int i=0; i<myTypes.size(); i++) {
        cout << "testDBE: registered type " << i
             << " - " << myTypes[i]
             << " - " << myTypes[i]->toString()
             << endl;
    }

    if ( dbmgr->connect() ) {
        DBEntity* cerca = dbmgr->getClazzByTypeName("DBESocieta");

        cerca->setValue("ragione_sociale","a");

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

        DBEntityVector* lista = dbmgr->Search(cerca,true,true,"ragione_sociale");
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
    delete dbmgr;
    delete mycon;
}
void testDBE(string host,string dbname,string usr,string pwd, string& loginUser, string& loginPwd) {
    string connString = string( "host="+host+" dbname="+dbname+" user="+usr+" password="+pwd );
    testDBE(connString, loginUser, loginPwd);
}

void testCRUD(string connString, string& loginUser, string& loginPwd) {
    DBLayer::Connection* con;
    DBMgr* dbmgr;

    con = DBLayer::createConnection( connString.c_str() );
    dbmgr = new DBLayer::DBMgr(con, false);

    //int fieldCreati = SchemaNS::getFieldCreati();
    //int fieldDistrutti = SchemaNS::getFieldDistrutti();
    //int schemiCreati = SchemaNS::getSchemiCreati();
    //int schemiDistrutti = SchemaNS::getSchemiDistrutti();
    //printf("::testCRUD: Field Creati: %d - Distrutti: %d; Schemi Creati: %d - Distrutti: %d\n",   SchemaNS::getFieldCreati() - fieldCreati, SchemaNS::getFieldDistrutti() - fieldDistrutti, SchemaNS::getSchemiCreati() - schemiCreati, SchemaNS::getSchemiDistrutti() - schemiDistrutti );

    DBEFactory dbeFactory(false);
    dbmgr->setDBEFactory(&dbeFactory);
    //dbmgr->setSchema("rra");
    AuthSchema::registerClasses(&dbeFactory);
    AuthSchema::checkDB(*dbmgr);
    TestSchema::registerClasses(&dbeFactory);
    TestSchema::checkDB(*dbmgr);

    //printf("::testCRUD: Field Creati: %d - Distrutti: %d; Schemi Creati: %d - Distrutti: %d\n",   SchemaNS::getFieldCreati() - fieldCreati, SchemaNS::getFieldDistrutti() - fieldDistrutti, SchemaNS::getSchemiCreati() - schemiCreati, SchemaNS::getSchemiDistrutti() - schemiDistrutti );

    if ( dbmgr->connect() ) {
        if(loginUser.length()>0 && loginPwd.length()>0) {
            dbmgr->login(loginUser,loginPwd);
        }

        if(dbmgr->isLoggedIn()) {

            DBEntity* nuova = dbmgr->getClazz("societa");
            nuova->setValue("ragione_sociale","Nuova Societa S.r.l.");
            nuova->setDateValue("data_creazione","1970-02-01 00:00:00");

            cout << "testCRUD: da inserire " << nuova->toString() << endl;
            //printf("::testCRUD: Field Creati: %d - Distrutti: %d; Schemi Creati: %d - Distrutti: %d\n",   SchemaNS::getFieldCreati() - fieldCreati, SchemaNS::getFieldDistrutti() - fieldDistrutti, SchemaNS::getSchemiCreati() - schemiCreati, SchemaNS::getSchemiDistrutti() - schemiDistrutti );

            // Insert
            cout << endl;
            cout << "testCRUD: provo ad inserire una nuova dbe " << endl;
            nuova = dbmgr->Insert(nuova);
            cout << "testCRUD: inserita nuova dbe " << nuova->toString() << endl;
            //printf("::testCRUD: Field Creati: %d - Distrutti: %d; Schemi Creati: %d - Distrutti: %d\n",   SchemaNS::getFieldCreati() - fieldCreati, SchemaNS::getFieldDistrutti() - fieldDistrutti, SchemaNS::getSchemiCreati() - schemiCreati, SchemaNS::getSchemiDistrutti() - schemiDistrutti );

            DBEntity* cerca = dbmgr->getClazz("societa");
            cerca->setValue("id", nuova->getStringValue("id") );
            cout << "testCRUD: cerca=" << cerca->toString() << endl;
            //printf("::testCRUD: Field Creati: %d - Distrutti: %d; Schemi Creati: %d - Distrutti: %d\n",   SchemaNS::getFieldCreati() - fieldCreati, SchemaNS::getFieldDistrutti() - fieldDistrutti, SchemaNS::getSchemiCreati() - schemiCreati, SchemaNS::getSchemiDistrutti() - schemiDistrutti );

            cout << endl;
            DBEntityVector* lista = dbmgr->Search(cerca,true,true,"id");
            if ( lista->size()>0 ) {
                cout << "testCRUD: Lista (" << DBLayer::integer2string((long)lista->size()) << "):" << endl;
                for(const auto& elem : (*lista)) {
                    cout << "- " << elem->toString() << endl;
                }
            } else {
                cout << "testCRUD: EMPTY LIST!!!" << endl;
            }
            dbmgr->Destroy(lista);
            //printf("::testCRUD: Field Creati: %d - Distrutti: %d; Schemi Creati: %d - Distrutti: %d\n",   SchemaNS::getFieldCreati() - fieldCreati, SchemaNS::getFieldDistrutti() - fieldDistrutti, SchemaNS::getSchemiCreati() - schemiCreati, SchemaNS::getSchemiDistrutti() - schemiDistrutti );

            // Update
            nuova->setValue("ragione_sociale","Nuova Societa L\'Attico S.r.l.");
            nuova->setValue("cap", "60015");

            cout << endl;
            nuova = dbmgr->Update(nuova);
            cout << "testCRUD: modificata nuova dbe " << nuova->toString() << endl;
            //printf("::testCRUD: Field Creati: %d - Distrutti: %d; Schemi Creati: %d - Distrutti: %d\n",   SchemaNS::getFieldCreati() - fieldCreati, SchemaNS::getFieldDistrutti() - fieldDistrutti, SchemaNS::getSchemiCreati() - schemiCreati, SchemaNS::getSchemiDistrutti() - schemiDistrutti );

            cout << endl;
            lista = dbmgr->Search(cerca,true,true,"id");
            if ( lista->size()>0 ) {
                cout << "testCRUD: Lista (" << DBLayer::integer2string((long)lista->size()) << "):" << endl;
                for(const auto& elem : (*lista)) {
                    cout << "- " << elem->toString() << endl;
                }
            } else {
                cout << "testCRUD: LISTA VUOTA!!!" << endl;
            }
            dbmgr->Destroy(lista);
            //printf("::testCRUD: Field Creati: %d - Distrutti: %d; Schemi Creati: %d - Distrutti: %d\n",   SchemaNS::getFieldCreati() - fieldCreati, SchemaNS::getFieldDistrutti() - fieldDistrutti, SchemaNS::getSchemiCreati() - schemiCreati, SchemaNS::getSchemiDistrutti() - schemiDistrutti );

            // Copy
            cout << endl;
            DBEntity* copia = dbmgr->Copy( nuova );
            if(copia!=0) {
                cout << "testCRUD: creata copia dbe " << copia->toString() << endl;
            } else {
                cout << "testCRUD: FALLITA copia" << endl;
            }
            //printf("::testCRUD: Field Creati: %d - Distrutti: %d; Schemi Creati: %d - Distrutti: %d\n",   SchemaNS::getFieldCreati() - fieldCreati, SchemaNS::getFieldDistrutti() - fieldDistrutti, SchemaNS::getSchemiCreati() - schemiCreati, SchemaNS::getSchemiDistrutti() - schemiDistrutti );

            cout << endl;
            DBEntity* cercaCopia = dbmgr->getClazz("societa");
            cercaCopia->setValue("ragione_sociale","Societa");
            lista = dbmgr->Search(cercaCopia, true, true,"id");
            if ( lista->size()>0 ) {
                cout << "testCRUD: Lista (" << DBLayer::integer2string((long)lista->size()) << "):" << endl;
                for(const auto& elem : (*lista)) {
                    cout << "- " << elem->toString() << endl;
                }
            } else {
                cout << "testCRUD: LISTA VUOTA!!!" << endl;
            }
            dbmgr->Destroy(lista);
            //printf("::testCRUD: Field Creati: %d - Distrutti: %d; Schemi Creati: %d - Distrutti: %d\n",   SchemaNS::getFieldCreati() - fieldCreati, SchemaNS::getFieldDistrutti() - fieldDistrutti, SchemaNS::getSchemiCreati() - schemiCreati, SchemaNS::getSchemiDistrutti() - schemiDistrutti );


            // Delete
            cout << endl;
            nuova = dbmgr->Delete(nuova);
            cout << "testCRUD: cancellata nuova dbe " << nuova->toString() << endl;
            cout << endl;
            if(copia!=0) {
                copia = dbmgr->Delete(copia);
                cout << "testCRUD: cancellata copia dbe " << copia->toString() << endl;
            }
            //printf("::testCRUD: Field Creati: %d - Distrutti: %d; Schemi Creati: %d - Distrutti: %d\n",   SchemaNS::getFieldCreati() - fieldCreati, SchemaNS::getFieldDistrutti() - fieldDistrutti, SchemaNS::getSchemiCreati() - schemiCreati, SchemaNS::getSchemiDistrutti() - schemiDistrutti );

            cout << endl;
            dbmgr->setVerbose(false);
            lista = dbmgr->Search(cercaCopia,true,true,"id");
            dbmgr->setVerbose(false);
            if ( lista->size()>0 ) {
                cout << "testCRUD: Lista (" << DBLayer::integer2string((long)lista->size()) << "):" << endl;
                for(const auto& elem : (*lista)) {
                    cout << "- " << elem->toString() << endl;
                }
            } else {
                cout << "testCRUD: LISTA VUOTA!!!" << endl;
            }
            dbmgr->Destroy(lista);
            //printf("::testCRUD: Field Creati: %d - Distrutti: %d; Schemi Creati: %d - Distrutti: %d\n",   SchemaNS::getFieldCreati() - fieldCreati, SchemaNS::getFieldDistrutti() - fieldDistrutti, SchemaNS::getSchemiCreati() - schemiCreati, SchemaNS::getSchemiDistrutti() - schemiDistrutti );

            delete nuova;
            //printf("::testCRUD: Field Creati: %d - Distrutti: %d; Schemi Creati: %d - Distrutti: %d\n",   SchemaNS::getFieldCreati() - fieldCreati, SchemaNS::getFieldDistrutti() - fieldDistrutti, SchemaNS::getSchemiCreati() - schemiCreati, SchemaNS::getSchemiDistrutti() - schemiDistrutti );
            delete copia;
            //printf("::testCRUD: Field Creati: %d - Distrutti: %d; Schemi Creati: %d - Distrutti: %d\n",   SchemaNS::getFieldCreati() - fieldCreati, SchemaNS::getFieldDistrutti() - fieldDistrutti, SchemaNS::getSchemiCreati() - schemiCreati, SchemaNS::getSchemiDistrutti() - schemiDistrutti );
            delete cerca;
            //printf("::testCRUD: Field Creati: %d - Distrutti: %d; Schemi Creati: %d - Distrutti: %d\n",   SchemaNS::getFieldCreati() - fieldCreati, SchemaNS::getFieldDistrutti() - fieldDistrutti, SchemaNS::getSchemiCreati() - schemiCreati, SchemaNS::getSchemiDistrutti() - schemiDistrutti );
            delete cercaCopia;
        } else {
            cerr << "testCRUD: Login Error - " << dbmgr->getErrorMessage() << "." << endl;
        }
    } else {
        cerr << "testCRUD: ERRORE " << dbmgr->getErrorMessage() << endl;
    }
    //printf("::testCRUD: Field Creati: %d - Distrutti: %d; Schemi Creati: %d - Distrutti: %d\n",   SchemaNS::getFieldCreati() - fieldCreati, SchemaNS::getFieldDistrutti() - fieldDistrutti, SchemaNS::getSchemiCreati() - schemiCreati, SchemaNS::getSchemiDistrutti() - schemiDistrutti );
    delete dbmgr;
    //printf("::testCRUD: Field Creati: %d - Distrutti: %d; Schemi Creati: %d - Distrutti: %d\n",   SchemaNS::getFieldCreati() - fieldCreati, SchemaNS::getFieldDistrutti() - fieldDistrutti, SchemaNS::getSchemiCreati() - schemiCreati, SchemaNS::getSchemiDistrutti() - schemiDistrutti );
    delete con;
}
void testCRUD(string host,string dbname,string usr,string pwd, string& loginUser, string& loginPwd) {
    string connString = string( "host="+host+" dbname="+dbname+" user="+usr+" password="+pwd );
    testCRUD( connString, loginUser, loginPwd );
}

void testDBES() {
    DBEntity dbe;
    cout << dbe.toString("\n") << endl;

    DBEDBVersion dbversion;
    cout << dbversion.toString("\n") << endl;

    DBEUserGroup dbeug;
    cout << dbeug.toString("\n") << endl;

    DBEGroup dbeg;
    cout << dbeg.toString("\n") << endl;

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
    QSqliteConnection::registerClass();
    DBLayer::QXmlrpcConnection::registerClass();
#endif

    cout << "---------------->>  testDateField: start." << endl;
    testDateField();
    cout << "---------------->>  testDateField: end." << endl;
    cout << endl;

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

    cout << "---------------->>  testSearch: start." << endl;
    if(argc==5) {
        testSearch( host, dbname, usr, pwd, login_user, login_password );
    } else {
        testSearch( connString, login_user, login_password );
    }
    printf("Field Creati: %d\n",   SchemaNS::getFieldCreati() );
    printf("Field Distrutti: %d\n",SchemaNS::getFieldDistrutti() );
    printf("Schemi Creati: %d\n",   SchemaNS::getSchemiCreati() );
    printf("Schemi Distrutti: %d\n",SchemaNS::getSchemiDistrutti() );
    cout << "---------------->>  testSearch: end." << endl;
    cout << endl;

    cout << "---------------->>  testDBE: start." << endl;
    if(argc==5) {
        testDBE( host, dbname, usr, pwd, login_user, login_password );
    } else {
        testDBE( connString, login_user, login_password );
    }
    printf("Field Creati: %d\n",   SchemaNS::getFieldCreati() );
    printf("Field Distrutti: %d\n",SchemaNS::getFieldDistrutti() );
    printf("Schemi Creati: %d\n",   SchemaNS::getSchemiCreati() );
    printf("Schemi Distrutti: %d\n",SchemaNS::getSchemiDistrutti() );
    cout << "---------------->>  testDBE: end." << endl;
    cout << endl;

    cout << "---------------->>  testCRUD: start." << endl;
    if(argc==5) {
        testCRUD( host, dbname, usr, pwd, login_user, login_password );
    } else {
        testCRUD( connString, login_user, login_password );
    }

    printf("Field Creati: %d\n",   SchemaNS::getFieldCreati() );
    printf("Field Distrutti: %d\n",SchemaNS::getFieldDistrutti() );
    printf("Schemi Creati: %d\n",   SchemaNS::getSchemiCreati() );
    printf("Schemi Distrutti: %d\n",SchemaNS::getSchemiDistrutti() );
    cout << "---------------->>  testCRUD: end." << endl;
    cout << endl;

    return EXIT_SUCCESS;
}
