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
#include "dblayer/datefield.h"
using namespace DBLayer;

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

bool testDBConnection(string& connString, string& loginUser, string& loginPwd) {
    bool success = true;
    DBLayer::Connection* con;
    DBLayer::ResultSet* res;

    con = DBLayer::createConnection( connString.c_str() );
    success = con->connect();

    if(loginUser.length()>0 && loginPwd.length()>0) {
        con->login(loginUser,loginPwd);
    }

    if ( !con->hasErrors() ) {
        if(success) {
            string myquery("select * from test_societa order by id desc");
            res = con->exec(myquery);
            //res = con->exec(string("select * from kware_users order by id desc"));
            //res = con->exec(string("select * from test_test_dblayer order by id desc"));
            //res = con->exec(string("select * from test_societa"));
            //res = con->exec(string("select * from rra_users order by id desc"));
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
                success = false;
                cout << "Errors: " << con->getErrorMessage() << endl;
            }
            delete res;
        }

        if(success) {
            string myquery = "select * from test_test_dblayer order by id desc";
            res = con->exec(myquery);
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
                success = false;
                cout << "Errors: " << con->getErrorMessage() << endl;
            }
            delete res;
        }
    } else {
        cout << "(testDBConnection) Errors: " << con->getErrorMessage() << endl;
    }
    //con->disconnect();
    delete con;
    return success;
}
bool testDBConnection(string host,string dbname,string usr,string pwd, string& loginUser, string& loginPwd) {
    string connString = string( "host="+host+" dbname="+dbname+" user="+usr+" password="+pwd );
    return testDBConnection( connString, loginUser, loginPwd );
}

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

void testGetKeys(string connString, string& loginUser, string& loginPwd, string relname) {
    DBLayer::Connection* con;

    con = DBLayer::createConnection( connString.c_str() );
    con->connect();
    //con->setVerbose(true);

    if(loginUser.length()>0 && loginPwd.length()>0) {
        con->login(loginUser,loginPwd);
    }

    if ( !con->hasErrors() ) {
        cout << "testGetKeys: table=" << relname << endl;

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
            cerr << "testGetKeys: Errors: " << con->getErrorMessage() << endl;
        }
    } else {
        cerr << "testGetKeys: Errors: " << con->getErrorMessage() << endl;
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
    //con->setVerbose(true);

    if(loginUser.length()>0 && loginPwd.length()>0) {
        con->login(loginUser,loginPwd);
    }

    if ( !con->hasErrors() ) {
        cout << "testGetForeignKeys: table=" << relname << endl;
        int numColonne = con->getColumnSize( &relname );
        cout << "\tNum. Colonne: " << numColonne << endl;
        for(int c = 1; c<=numColonne; c++) {
            cout << "\t" << c << ") " << con->getColumnName( &relname, c ) << endl;
        }
        DBLayer::IntegerVector chiavi = con->getForeignKeys(&relname);
        if( !con->hasErrors() ) {
            unsigned int nChiavi = (unsigned int) chiavi.size();
            for(unsigned int i=0; i<nChiavi; i++) {
                cout << "testGetForeignKeys: foreign key column:\t" << chiavi[i] << "\t"
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


void testGetColumnsForTable(string connString, string& loginUser, string& loginPwd, string relname) {
    DBLayer::Connection* con;

    con = DBLayer::createConnection( connString.c_str() );
    con->setVerbose(true);

    if ( con->connect() ) {

        bool isLoggedIn = false;
        if(loginUser.length()>0 && loginPwd.length()>0) {
            ResultSet* rs = con->login(loginUser,loginPwd);
            isLoggedIn = rs->getNumRows()>0;
        }

        if(isLoggedIn) {
            cout << endl;
            cout << "testGetColumnsForTable: fetching definitions for table " << relname << endl;
            ColumnDefinitions cols = con->getColumnsForTable(relname);

            for(const auto& elem : cols) {
                cout << elem.first << endl;
                string glue(",");
                cout << "  " << joinString( (DBLayer::StringVector*) &(elem.second),&glue) << endl;
            }
        } else {
            cerr << "testGetColumnsForTable: Login error - " << con->getErrorMessage() << "." << endl;
        }
    } else {
        cerr << "testGetColumnsForTable: ERROR " << con->getErrorMessage() << endl;
    }
    delete con;
}
void testGetColumnsForTable(string host,string dbname,string usr,string pwd, string& loginUser, string& loginPwd, string relname) {
    string connString = string( "host="+host+" dbname="+dbname+" user="+usr+" password="+pwd );
    testGetColumnsForTable( connString, loginUser, loginPwd, relname );
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
    QXmlrpcConnection::registerClass();
#endif
    bool success = true;

    cout << "---------------->>  testDateField: start." << endl;
    testDateField();
    cout << "---------------->>  testDateField: end." << endl;
    cout << endl;

    cout << "---------------->>  testDBConnection: start." << endl;
    if( argc==5 ) {
        success = testDBConnection( host, dbname, usr, pwd, login_user, login_password );
    } else {
        success = testDBConnection( connString, login_user, login_password );
    }
    printf("Field Creati: %d\n",   SchemaNS::getFieldCreati() );
    printf("Field Distrutti: %d\n",SchemaNS::getFieldDistrutti() );
    printf("Schemi Creati: %d\n",   SchemaNS::getSchemiCreati() );
    printf("Schemi Distrutti: %d\n",SchemaNS::getSchemiDistrutti() );
    cout << "---------------->>  testDBConnection: end." << endl;
    cout << endl;
    if(!success) {
        cerr << "TEST FAILED!!!" << endl;
        return 1;
    }

    cout << "---------------->>  testGetKeys: start." << endl;
    if( argc==5 ) {
        testGetKeys( host, dbname, usr, pwd, login_user, login_password, "test_test_dblayer" );
        testGetKeys( host, dbname, usr, pwd, login_user, login_password, "test_societa" );
    } else {
        testGetKeys( connString, login_user, login_password, "test_test_dblayer" );
        testGetKeys( connString, login_user, login_password, "test_societa" );
    }
    cout << "---------------->>  testGetKeys: end." << endl;
    cout << endl;
return 0;
    cout << "---------------->>  testGetForeignKeys: start." << endl;
    if( argc==5 ) {
        testGetForeignKeys( host, dbname, usr, pwd, login_user, login_password, "test_test_dblayer" );
        testGetForeignKeys( host, dbname, usr, pwd, login_user, login_password, "test_societa" );
    } else {
        testGetForeignKeys( connString, login_user, login_password, "test_test_dblayer" );
        testGetForeignKeys( connString, login_user, login_password, "test_societa" );
    }
    cout << "---------------->>  testGetForeignKeys: end." << endl;
    cout << endl;

    cout << "---------------->>  testGetColumnsForTable: start." << endl;
    if( argc==5 ) {
        testGetColumnsForTable( host, dbname, usr, pwd, login_user, login_password, "test_test_dblayer" );
        testGetColumnsForTable( host, dbname, usr, pwd, login_user, login_password, "test_societa" );
        testGetColumnsForTable( host, dbname, usr, pwd, login_user, login_password, "auth_users" );
    } else {
        testGetColumnsForTable( connString, login_user, login_password, "test_test_dblayer" );
        testGetColumnsForTable( connString, login_user, login_password, "test_societa" );
        testGetColumnsForTable( connString, login_user, login_password, "auth_users" );
    }
    cout << "---------------->>  testGetColumnsForTable: end." << endl;
    cout << endl;

    return EXIT_SUCCESS;
}
