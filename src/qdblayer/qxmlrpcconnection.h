#ifndef QXMLRPCCONNECTION_H
#define QXMLRPCCONNECTION_H

#include <QtCore/QEventLoop>
#include <QtCore/QMap>
#include <QtCore/QMutex>
#include <QtCore/QObject>

#include <dblayer/dblayer.h>
#include <dblayer/dbconnection.h>
#include <dblayer/dbentity.h>
#include "qdblayer.h"
#include "qresultset.h"
using namespace DBLayer;

//#include <xrclient.h>
class XmlRpcClient;
namespace DBLayer {

class QXmlrpcConnection : public QObject , public Connection {
    Q_OBJECT

  public:
    QXmlrpcConnection(string s,QObject* parent=0);
    virtual ~QXmlrpcConnection();

    virtual bool connect();
    virtual bool disconnect();

    virtual ResultSet* exec(const string s);
    /** Chiude la connessione corrente e la riapre */
    virtual bool reconnect();

    virtual string escapeString(string s) const;

    virtual int getColumnSize(string* relname);
    virtual string getColumnName(string* relname, int column);
    virtual IntegerVector getKeys(string* relname);
    virtual IntegerVector getForeignKeys(string* relname);
    virtual ColumnDefinitions getColumnsForTable(const string& tablename);

    virtual ResultSet* login(string user, string pwd);

    virtual string getFormSchema(string language="python");
    virtual string getDBSchema(string language="python");
    virtual string getSchemaName();
    inline string getDBType() { return "QXmlrpc"; }

    static QString variant2string(const QVariant& v, QString prefix="");
    /* Non mi piacciono i parametri in input usati anche per l'output,
        per questo ritorno lo stesso resultset passato in argomento */
    static QResultSet* list2resultset(QList<QVariant>* iLista, QResultSet* ioResultSet);

    XmlRpcClient* getXmlrpcClient();
    //void setXmlrpcClient(XRClient* c); // RRA - NO!

    // **************** Proxy Connections :: 20091015: start. *********************
    // The proxy connections are used by DBMgr to execute the following methods
    virtual bool isProxy() const;
    virtual DBEntity* Insert(DBEntity *dbe);
    virtual DBEntity* Update(DBEntity *dbe);
    virtual DBEntity* Delete(DBEntity *dbe);
    virtual DBEntityVector* Select(DBEntity* dbe, const string& tableName, const string& searchString);
    virtual DBEntityVector* Search(DBEntity* dbe, bool uselike=true, bool caseSensitive=true, const string& orderBy="" );
    virtual string ping();
  private:
    QList<QVariant>* _dbeToVariant(DBEntity* dbe, QList<QVariant>* ioVariant);
    DBEntity* _variantToDBE(QVariant* v, DBEntity* ioDbe);
    // **************** Proxy Connections :: 20091015: end. *********************

  public slots:

  signals:

  private:
    // XmlRpc
    string myUrl;
    XmlRpcClient* myClient;

    QMap<int, QString> _req_to_method;
    QMap<int, QVariant*> _request_responses;
    QMap<int, QResultSet*> _resultsets;

    /* DBEUser */
    DBEntity* user;

  public:
    static void registerClass();
};

    Connection* createQXmlrpcConnection(string s);
}

#endif // QXMLRPCCONNECTION_H
