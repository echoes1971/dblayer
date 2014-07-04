#include "../xrclient.h"
#include <iostream>
#include <QApplication>
//#include "clientimpl.h"


int main(int argc, char* argv[])
{
  QApplication app(argc,argv);
  QUrl server_url("http://rproject.ath.cx/rproject/xmlrpc_server.php");
  XRClient client(server_url, &app, true);
  //ClientImpl client;
  return app.exec();
}
