#include <validationserver.h>
#include <qapplication.h>

int main(int argc, char* argv[])
{
  //This is not a gui app
  QApplication app(argc,argv,false);
  //Make the server
  XRValidationServer server(8001);
  //Enter the event loop
  return app.exec();
  
}
