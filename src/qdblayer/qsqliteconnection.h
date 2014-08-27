#ifndef QSQLITECONNECTION_H
#define QSQLITECONNECTION_H

#include <QtCore/QEventLoop>
#include <QtCore/QMap>
#include <QtCore/QMutex>
#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtSql/QSqlDatabase>

#include <dblayer/dblayer.h>
#include <dblayer/dbconnection.h>
#include <dblayer/dbentity.h>
#include "qdblayer.h"
#include "qresultset.h"
using namespace DBLayer;

namespace DBLayer {

//class QSqliteConnection : public QObject , public Connection {
//    Q_OBJECT
class QSqliteConnection : public Connection {

  public:
    QSqliteConnection(string s);
    //QSqliteConnection(string s,QObject* parent=0);
    virtual ~QSqliteConnection();

    virtual bool connect();
    virtual bool disconnect();

    virtual ResultSet* exec(const string s);
    /** Chiude la connessione corrente e la riapre */
    virtual bool reconnect();

    virtual string escapeString(string s) const;

    virtual ColumnDefinitions getColumnsForTable(const string& tablename);
    virtual int getColumnSize(string* relname);
    virtual string getColumnName(string* relname, int column);
//     virtual IntegerVector getKeys(string* relname);
//     virtual IntegerVector getForeignKeys(string* relname);
    inline string getDBType() { return "QSqlite"; }

    //virtual string dbeType2dbType(const string& dbetype);

    static QString variant2string(const QVariant& v, QString prefix="");
    /* Non mi piacciono i parametri in input usati anche per l'output,
        per questo ritorno lo stesso resultset passato in argomento */
    static QResultSet* list2resultset(QList<QVariant>* iLista, QResultSet* ioResultSet);

    static void registerClass();

  private:
    string dbname;
    QSqlDatabase db;
};

    Connection* createQSqliteConnection(string s);
}

#endif // QSQLITECONNECTION_H
