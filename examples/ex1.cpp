/**
    Example 1
    =========

    Description:
        open a connection to the local sqlite db (test.db) and performs a simple query.

    Compile:
        g++ -ldblayer -o ex1 ex1.cpp
    and execute
        ./ex1

*/

#include <iostream>
using namespace std;


#include <dblayer/dbconnection.h>
using namespace DBLayer;


int main(int argc, char* argv[]) {

  string connString("dblayer:sqlite:./test.db");

  DBLayer::Connection* con;
  DBLayer::ResultSet* res;

  con = DBLayer::createConnection( connString.c_str() );
  con->connect();

  if(!con->hasErrors()) {
    res = con->exec("select * from test_dblayer order by id desc");

    if( !con->hasErrors() ) {
      cout << res->toString() << endl;
    } else {
      cout << "Exec error: " << con->getErrorMessage() << endl;
    }

    delete res;
  } else {
    cout << "Connection error: " << con->getErrorMessage() << endl;
  }

  delete con;

  return 0;
}

