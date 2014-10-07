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

#include "dblayer/cmschema.h"
using namespace CMSchema;

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


bool testTemplate(string connString, string& loginUser, string& loginPwd) {
    bool success = true;
    DBLayer::Connection* con;
    ObjectMgr* objmgr;

    con = DBLayer::createConnection( connString.c_str() );
    //con->setVerbose(true);
    objmgr = new ObjectMgr(con, false);

    DBEFactory* dbeFactory = new DBEFactory(false);
    objmgr->setDBEFactory(dbeFactory);
    AuthSchema::registerClasses(dbeFactory);
    AuthSchema::checkDB(*objmgr,false);
    CMSchema::registerClasses(dbeFactory);
    CMSchema::checkDB(*objmgr,false);

    if(objmgr->connect()) {

        if(loginUser.length()>0 && loginPwd.length()>0) {
            objmgr->login(loginUser,loginPwd);
        }

        if(objmgr->isLoggedIn()) {

            // TODO do something here

        } else {
            cout << "Login Error: " << objmgr->getErrorMessage() << "." << endl;
            success = false;
        }
    } else {
        cout << "Connection Error: " << objmgr->getErrorMessage() << endl;
        success = false;
    }
    delete dbeFactory;
    delete objmgr;
    delete con;
    success = success && SchemaNS::getCreatedSchema().size()==0;
    return success;
}
bool testTemplate(string host,string dbname,string usr,string pwd, string& loginUser, string& loginPwd) {
    string connString = string( "host="+host+" dbname="+dbname+" user="+usr+" password="+pwd );
    return testTemplate( connString, loginUser, loginPwd );
}


bool testObjectMgr(string connString, string& loginUser, string& loginPwd) {
    bool success = true;
    DBLayer::Connection* con;
    ObjectMgr* objmgr;

    con = DBLayer::createConnection( connString.c_str() );
    //con->setVerbose(true);
    objmgr = new ObjectMgr(con, false);

    DBEFactory* dbeFactory = new DBEFactory(false);
    objmgr->setDBEFactory(dbeFactory);
    AuthSchema::registerClasses(dbeFactory);
    AuthSchema::checkDB(*objmgr,false);
    CMSchema::registerClasses(dbeFactory);
    CMSchema::checkDB(*objmgr,false);

    if(objmgr->connect()) {

        if(loginUser.length()>0 && loginPwd.length()>0) {
            objmgr->login(loginUser,loginPwd);
        }

        if(objmgr->isLoggedIn()) {

            DBEntity* search = objmgr->getClazz("dbversion");
            objmgr->setVerbose(true);
            DBEntityVector* lista = objmgr->Search(search,true,true,"model_name");
            objmgr->setVerbose(false);
            if ( lista->size()>0 ) {
                cout << "testObjectMgr: Lista (" << DBLayer::integer2string((long)lista->size()) << "):" << endl;
                for(const DBEntity* elem : (*lista)) {
                    cout << "- " << elem->toString() << endl;
                }
            } else {
                success = false;
                cout << "testObjectMgr: EMPTY LIST!!!" << endl;
            }
            objmgr->Destroy(lista);
            delete search;

//             // Let's create an object instance
//             DBEObject* obj = (DBEObject*) objmgr->getClazz("objects");
// 
//             obj->setDefaultValues(objmgr);
//             cout << "Object: " << obj->toString("\n") << endl;
// 
//             cout << "Has group " << obj->getGroupId() << ": " << objmgr->hasGroup(obj->getGroupId()) << endl;
//             cout << "Can read: " << objmgr->canRead(*obj) << endl;
//             cout << "Can write: " << objmgr->canWrite(*obj) << endl;
//             cout << "Can execute: " << objmgr->canExecute(*obj) << endl;
//             
//             cout << "objmgr.Select: TODO" << endl;
// 
//             delete obj;
// 
//             printf("\n");
//             printf("Field Creati: %d\n",   SchemaNS::getFieldCreati() );
//             printf("Field Distrutti: %d\n",SchemaNS::getFieldDistrutti() );
//             printf("Schemi Creati: %d\n",   SchemaNS::getSchemiCreati() );
//             printf("Schemi Distrutti: %d\n",SchemaNS::getSchemiDistrutti() );
//             printf("\n");
        } else {
            cout << "Login Error: " << objmgr->getErrorMessage() << "." << endl;
            success = false;
        }
    } else {
        cout << "Connection Error: " << objmgr->getErrorMessage() << endl;
        success = false;
    }
    delete dbeFactory;
    delete objmgr;
    delete con;
    success = success && SchemaNS::getCreatedSchema().size()==0;
    return success;
}
bool testObjectMgr(string host,string dbname,string usr,string pwd, string& loginUser, string& loginPwd) {
    string connString = string( "host="+host+" dbname="+dbname+" user="+usr+" password="+pwd );
    return testObjectMgr( connString, loginUser, loginPwd );
}


bool testDBES() {
    bool success=true;

//    DBEntity dbe;
//    cout << dbe.toString("\n") << endl;

//    DBEDBVersion dbversion;
//    cout << dbversion.toString("\n") << endl;

//    DBEUserGroup dbeug;
//    cout << dbeug.toString("\n") << endl;

//    DBEGroup dbeg;
//    cout << dbeg.toString("\n") << endl;

//    DBEUser dbeu;
//    cout << dbeu.toString("\n") << endl;

    DBELog* dbelog = new DBELog();
    cout << dbelog->toString("\n") << endl;
    delete dbelog;

    DBEObject* dbeobject = new DBEObject();
    cout << dbeobject->toString("\n") << endl;

    cout << "Owner ID:\t" << dbeobject->getOwnerId() << " => ";
    dbeobject->setValue("owner","cippa");
    cout << dbeobject->getOwnerId() << endl;

    cout << "Group ID:\t" << dbeobject->getGroupId() << " => ";
    dbeobject->setValue("group_id","lippa");
    cout << dbeobject->getGroupId() << endl;

    cout << "Deleted:\t" << dbeobject->isDeleted() << " => ";
    dbeobject->setValue("deleted_date","lippa");
    cout << dbeobject->isDeleted() << endl;

    dbeobject->setValue("permissions","r---w---x");
    //dbeobject->setValue("permissions","rwx------");
    cout << "User  can read: " << dbeobject->canRead("U") << endl;
    cout << "Group can read: " << dbeobject->canRead("G") << endl;
    cout << "All   can read: " << dbeobject->canRead("A") << endl;
    cout << "User  can write: " << dbeobject->canWrite("U") << endl;
    cout << "Group can write: " << dbeobject->canWrite("G") << endl;
    cout << "All   can write: " << dbeobject->canWrite("A") << endl;
    cout << "User  can exec: " << dbeobject->canExecute("U") << endl;
    cout << "Group can exec: " << dbeobject->canExecute("G") << endl;
    cout << "All   can exec: " << dbeobject->canExecute("A") << endl;
    //success = false;

    delete dbeobject;

    success = success && SchemaNS::getCreatedSchema().size()==0;

    return success;
}


bool testCRUD(string tablename, string connString, string& loginUser, string& loginPwd) {
    bool success = true;
    DBLayer::Connection* con;
    ObjectMgr* objmgr;

    con = DBLayer::createConnection( connString.c_str() );
    //con->setVerbose(true);
    objmgr = new ObjectMgr(con, false);

    DBEFactory* dbeFactory = new DBEFactory(false);
    objmgr->setDBEFactory(dbeFactory);
    AuthSchema::registerClasses(dbeFactory);
    AuthSchema::checkDB(*objmgr,true);
    CMSchema::registerClasses(dbeFactory);
    CMSchema::checkDB(*objmgr,false);

    if(objmgr->connect()) {

        if(loginUser.length()>0 && loginPwd.length()>0) {
            objmgr->login(loginUser,loginPwd);
        }

        if(objmgr->isLoggedIn()) {

            // TODO do something here

        } else {
            cout << "Login Error: " << objmgr->getErrorMessage() << "." << endl;
            success = false;
        }
    } else {
        cout << "Connection Error: " << objmgr->getErrorMessage() << endl;
        success = false;
    }
    delete dbeFactory;
    delete objmgr;
    delete con;
    success = success && SchemaNS::getCreatedSchema().size()==0;
    return success;
}
bool testCRUD(string tablename, string host,string dbname,string usr,string pwd, string& loginUser, string& loginPwd) {
    string connString = string( "host="+host+" dbname="+dbname+" user="+usr+" password="+pwd );
    return testCRUD(tablename, connString, loginUser, loginPwd );
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
    bool success = true;

    cout << "---------------->>  testDBES: start." << endl;
    success = testDBES();
    printf("\n");
    printf("Field Creati: %d\n",   SchemaNS::getFieldCreati() );
    printf("Field Distrutti: %d\n",SchemaNS::getFieldDistrutti() );
    printf("Schemi Creati: %d\n",   SchemaNS::getSchemiCreati() );
    printf("Schemi Distrutti: %d\n",SchemaNS::getSchemiDistrutti() );
    if(SchemaNS::getCreatedSchema().size()>0) {
        printf("Schemas left:\n");
        for(const Schema* schema : SchemaNS::getCreatedSchema()) {
            printf("%ld", (unsigned long) schema );
            cout << schema->toString(" \n") << endl;
        }
    }
    cout << "---------------->>  testDBES: end." << endl;
    cout << endl;
    if(!success) {
        cerr << "TEST FAILED!!!" << endl;
        return 1;
    }

    cout << "---------------->>  testObjectMgr: start." << endl;
    if ( argc==5 ) {
        success = testObjectMgr( host, dbname, usr, pwd, login_user, login_password );
    } else {
        success = testObjectMgr( connString, login_user, login_password );
    }
    printf("\n");
    printf("Field Creati: %d\n",   SchemaNS::getFieldCreati() );
    printf("Field Distrutti: %d\n",SchemaNS::getFieldDistrutti() );
    printf("Schemi Creati: %d\n",   SchemaNS::getSchemiCreati() );
    printf("Schemi Distrutti: %d\n",SchemaNS::getSchemiDistrutti() );
    if(SchemaNS::getCreatedSchema().size()>0) {
        printf("Schemas left:\n");
        for(const Schema* schema : SchemaNS::getCreatedSchema()) {
            printf("%ld", (unsigned long) schema );
            cout << schema->toString(" \n") << endl;
        }
    }
    cout << "---------------->>  testObjectMgr: end." << endl;
    cout << endl;
    if(!success) {
        cerr << "TEST FAILED!!!" << endl;
        return 1;
    }

    cout << "---------------->>  testCRUD: start." << endl;
    if ( argc==5 ) {
        success = testCRUD( "people", host, dbname, usr, pwd, login_user, login_password );
    } else {
        success = testCRUD( "people", connString, login_user, login_password );
    }
    printf("\n");
    printf("Field Creati: %d\n",   SchemaNS::getFieldCreati() );
    printf("Field Distrutti: %d\n",SchemaNS::getFieldDistrutti() );
    printf("Schemi Creati: %d\n",   SchemaNS::getSchemiCreati() );
    printf("Schemi Distrutti: %d\n",SchemaNS::getSchemiDistrutti() );
    if(SchemaNS::getCreatedSchema().size()>0) {
        printf("Schemas left:\n");
        for(const Schema* schema : SchemaNS::getCreatedSchema()) {
            printf("%ld", (unsigned long) schema );
            cout << schema->toString(" \n") << endl;
        }
    }
    cout << "---------------->>  testCRUD: end." << endl;
    cout << endl;
    if(!success) {
        cerr << "TEST FAILED!!!" << endl;
        return 1;
    }

    return EXIT_SUCCESS;
}
