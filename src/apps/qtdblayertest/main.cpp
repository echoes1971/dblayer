#include <dblayer/dbefactory.h>
using namespace DBLayer;

#include <QtGui/QApplication>
#include "mainwindow.h"

#include "dbschema.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DBEFactory dbeFactory; //(true);
    MySchema::registerClasses(&dbeFactory);

    MainWindow w;
    w.dbeFactory = &dbeFactory;
    w.show();
    return a.exec();
}
