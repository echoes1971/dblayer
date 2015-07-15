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


bool testSchemas(string connString, string& loginUser, string& loginPwd) {
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

            DBEntity* search = 0;
            DBEntityVector* lista = 0;

            search = objmgr->getClazz("dbversion");
//             objmgr->setVerbose(true);
            lista = objmgr->Search(search,true,true,"model_name");
            objmgr->setVerbose(false);
            if ( lista->size()>0 ) {
                cout << "testSchemas: Lista (" << DBLayer::integer2string((long)lista->size()) << "):" << endl;
                for(const DBEntity* elem : (*lista)) {
                    cout << "- " << elem->toString() << endl;
                }
            } else {
                success = false;
                cout << "testSchemas: EMPTY LIST!!!" << endl;
            }
            objmgr->Destroy(lista);
            delete search;

            search = objmgr->getClazz("users");
//             objmgr->setVerbose(true);
            lista = objmgr->Search(search,true,true,"login");
            objmgr->setVerbose(false);
            if ( lista->size()>0 ) {
                cout << "testSchemas: Lista (" << DBLayer::integer2string((long)lista->size()) << "):" << endl;
                for(const DBEntity* elem : (*lista)) {
                    cout << "- " << elem->toString() << endl;
                }
            } else {
                success = false;
                cout << "testSchemas: EMPTY LIST!!!" << endl;
            }
            objmgr->Destroy(lista);
            delete search;

            search = objmgr->getClazz("groups");
//             objmgr->setVerbose(true);
            lista = objmgr->Search(search,true,true,"name");
            objmgr->setVerbose(false);
            if ( lista->size()>0 ) {
                cout << "testSchemas: Lista (" << DBLayer::integer2string((long)lista->size()) << "):" << endl;
                for(const DBEntity* elem : (*lista)) {
                    cout << "- " << elem->toString() << endl;
                }
            } else {
                success = false;
                cout << "testSchemas: EMPTY LIST!!!" << endl;
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
bool testSchemas(string host,string dbname,string usr,string pwd, string& loginUser, string& loginPwd) {
    string connString = string( "host="+host+" dbname="+dbname+" user="+usr+" password="+pwd );
    return testSchemas( connString, loginUser, loginPwd );
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
    AuthSchema::checkDB(*objmgr,false);
    CMSchema::registerClasses(dbeFactory);
    CMSchema::checkDB(*objmgr,false);

    if(objmgr->connect()) {

        if(loginUser.length()>0 && loginPwd.length()>0) {
            objmgr->login(loginUser,loginPwd);
        }

        if(objmgr->isLoggedIn()) {

            int counter = 1;
            string counter_str = SchemaNS::integer2string(counter);

            DBEObject* search = 0;
            DBEntityVector* lista = 0;

            DBEObject* dbeobject = (DBEObject*) objmgr->getClazz(tablename);
            dbeobject->setValue("name","Name " + counter_str);
            dbeobject->setValue("description","Description " + counter_str);

            cout << "testCRUD: to insert " << dbeobject->toString("\n ") << endl;
            //printf("::testCRUD: Field Creati: %d - Distrutti: %d; Schemi Creati: %d - Distrutti: %d\n",   SchemaNS::getFieldCreati() - fieldCreati, SchemaNS::getFieldDistrutti() - fieldDistrutti, SchemaNS::getSchemiCreati() - schemiCreati, SchemaNS::getSchemiDistrutti() - schemiDistrutti );

            // Create
            cout << "testCRUD: try to insert a new DBEObject" << endl;
//             objmgr->setVerbose(true);
            dbeobject = (DBEObject*) objmgr->Insert(dbeobject);
            objmgr->setVerbose(false);
            cout << "testCRUD: inserted new DBEObject " << dbeobject->toString("\n ") << endl;

            counter++; counter_str = SchemaNS::integer2string(counter);

            // Update
            cout << "testCRUD: try to update the DBEObject" << endl;
            dbeobject->setValue("name","Name " + counter_str);
            dbeobject->setValue("description","Description " + counter_str);
//             objmgr->setVerbose(true);
            dbeobject = (DBEObject*) objmgr->Update(dbeobject);
            objmgr->setVerbose(false);
            cout << "testCRUD: updated DBEObject " << dbeobject->toString("\n ") << endl;

            // Read
            search = (DBEObject*) objmgr->getClazz(tablename);
            search->setValue("name","Name " + counter_str);
            lista = objmgr->Search(search,true,true,"name");
            if ( lista->size()>0 ) {
                cout << "testCRUD: Lista (" << DBLayer::integer2string((long)lista->size()) << "):" << endl;
                for(const DBEntity* elem : (*lista)) cout << "- " << elem->toString("\n  ") << endl;
            } else {
                success = false;
                cout << "testCRUD: EMPTY LIST!!!" << endl;
            }
            objmgr->Destroy(lista);
            delete search;

            // Delete
            cout << "testCRUD: try to delete the DBEObject" << endl;
            //objmgr->setVerbose(true);
            con->setVerbose(true);
            dbeobject = (DBEObject*) objmgr->Delete(dbeobject);
            con->setVerbose(false);
            objmgr->setVerbose(false);
            success = success && dbeobject->isDeleted();
            if(dbeobject->isDeleted())
                dbeobject = (DBEObject*) objmgr->Delete(dbeobject);
            cout << "testCRUD: deleted the DBEObject " << dbeobject->toString("\n ") << endl;

            delete dbeobject;

            // Read
            search = (DBEObject*) objmgr->getClazz(tablename);
            search->setValue("name","Name " + counter_str);
            lista = objmgr->Search(search,true,true,"name");
            if ( lista->size()>0 ) {
                success = false;
                cout << "testCRUD: Lista (" << DBLayer::integer2string((long)lista->size()) << "):" << endl;
                for(const DBEntity* elem : (*lista)) cout << "- " << elem->toString("\n  ") << endl;
            } else {
                success = true;
                cout << "testCRUD: EMPTY LIST!!!" << endl;
            }
            objmgr->Destroy(lista);
            delete search;

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

bool testDBEFile(string connString, string& loginUser, string& loginPwd) {
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

            // Create people
            DBEPeople* people = new DBEPeople();
            people->setName("Lippo Lippi");
            people = (DBEPeople*) objmgr->Insert(people);
            cout << people->toString("\n") << endl;

            // Create folder
            DBEFolder* folder = new DBEFolder();
            folder->setName("Test Folder");
            folder->setValue("fk_obj_id",people->getId());
//             objmgr->setVerbose(true);
            folder = (DBEFolder*) objmgr->Insert(folder);
            objmgr->setVerbose(false);
            cout << folder->toString("\n") << endl;

            // Create a file
            cout << endl;
            DBEFile* dbefile = new DBEFile();
            dbefile->setRootDirectory("/home/roberto/tmp");
            dbefile->setValue("father_id",folder->getId());
            dbefile->setValue("path","my/path/to/object");
//             dbefile->setFilename("myfile.txt");
            success = success && dbefile->readFile("../examples/keepcalm.png",false);
            //success = success && dbefile->readFile("../examples/test.db",false);
            //success = success && dbefile->readFile("../examples/test2.db",true);
            dbefile = (DBEFile*) objmgr->Insert(dbefile);
            string err = objmgr->getErrorMessage();
            if(err.size()==0) {
                cout << "= Created =============================" << endl;
                cout << dbefile->toString("\n") << endl;
                cout << "createFilename: " << dbefile->createFilename() << endl;
                cout << "createObjectPath: " << dbefile->createObjectPath() << endl;
                cout << "getFullpath: " << dbefile->getFullpath() << endl;
                cout << "isImage: " << dbefile->isImage() << endl;
                
                // Update with other content
                dbefile->readFile("../examples/33_a.pdf",false);
                dbefile = (DBEFile*) objmgr->Update(dbefile);
                err = objmgr->getErrorMessage();
                if(err.size()==0) {
                    cout << "= Updated =============================" << endl;
                    cout << dbefile->toString("\n") << endl;
                    cout << "createFilename: " << dbefile->createFilename() << endl;
                    cout << "createObjectPath: " << dbefile->createObjectPath() << endl;
                    cout << "getFullpath: " << dbefile->getFullpath() << endl;
                    cout << "isImage: " << dbefile->isImage() << endl;
                } else {
                    cerr << "Update file error: " << err << endl;
                    success = false;
                }
                objmgr->Delete(dbefile);
                err = objmgr->getErrorMessage();
                if(err.size()>0) {
                    cerr << "Delete file error: " << err << endl;
                    success = false;
                }
                objmgr->Delete(dbefile);
                err = objmgr->getErrorMessage();
                if(err.size()>0) {
                    cerr << "Delete file error: " << err << endl;
                    success = false;
                }
                delete dbefile;
                cout << "= Deleted =============================" << endl;
                //success = false;
            } else {
                cerr << "Insert file error: " << err << endl;
                success = false;
            }

            objmgr->Delete(folder);
            delete folder;

            objmgr->Delete(people);
            delete people;

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
bool testDBEFile(string host,string dbname,string usr,string pwd, string& loginUser, string& loginPwd) {
    string connString = string( "host="+host+" dbname="+dbname+" user="+usr+" password="+pwd );
    return testDBEFile( connString, loginUser, loginPwd );
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

    cout << "---------------->>  testSchemas: start." << endl;
    if ( argc==5 ) {
        success = testSchemas( host, dbname, usr, pwd, login_user, login_password );
    } else {
        success = testSchemas( connString, login_user, login_password );
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
    cout << "---------------->>  testSchemas: end." << endl;
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

    cout << "---------------->>  testDBEFile: start." << endl;
    if ( argc==5 ) {
        success = testDBEFile( host, dbname, usr, pwd, login_user, login_password );
    } else {
        success = testDBEFile( connString, login_user, login_password );
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
    cout << "---------------->>  testDBEFile: end." << endl;
    cout << endl;
    if(!success) {
        cerr << "TEST FAILED!!!" << endl;
        return 1;
    }

    return EXIT_SUCCESS;
}
