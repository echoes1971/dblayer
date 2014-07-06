#include "../xrclient.h"
#include <iostream>
#include <QApplication>
//#include "clientimpl.h"


int main(int argc, char* argv[])
{
  QApplication app(argc,argv);
  //QUrl server_url("http://rproject.ath.cx/rproject/xmlrpc_server.php");
  QUrl server_url("http://127.0.0.1/~roberto/rp/xmlrpc_server.php");
  XRClient client(server_url, &app, true);

  QList<QVariant> params;
  QVariant resp = client.syncCall("ping",params);

  std::cout << "resp:" << resp.toString().toStdString() << std::endl;


  //ClientImpl client;
  return 0; //app.exec();
}
