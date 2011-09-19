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
using namespace DBLayer;

//#include "dbschema.h"
//using namespace MySchema;

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

    return EXIT_SUCCESS;
}
