#include <cstdio>

#include "qutexmlrpc/xrclient.h"

#include "qxmlrpcconnection.h"
#include "qresultset.h"
using namespace DBLayer;


Connection* DBLayer::createQXmlrpcConnection(string s) {
    return new QXmlrpcConnection(s);
}
void QXmlrpcConnection::registerClass() {
    //printf("DBLayer::__init_qxmlrpc: inizio.\n");
    DBLayer::registerConnectionType("qxmlrpc",&DBLayer::createQXmlrpcConnection);
    //printf("DBLayer::__init_qxmlrpc: fine.\n");
}

#ifdef WIN32
QXmlrpcConnection::QXmlrpcConnection(string s, QObject* parent) : QObject(parent), Connection(s) {
#else
QXmlrpcConnection::QXmlrpcConnection(string s, QObject* parent) : QObject(parent), Connection::Connection(s) {
#endif
    this->myUrl = s;
    this->myClient = 0;
}
QXmlrpcConnection::~QXmlrpcConnection() {
    this->disconnect();
    if(this->myClient!=0) delete this->myClient;
}

XmlRpcClient* QXmlrpcConnection::getXmlrpcClient() { return this->myClient; }

bool QXmlrpcConnection::connect() {
    //printf("%0x::QXmlrpcConnection::connect: start.\n",(int) QThread::currentThread());
    if(this->myClient==0) {
        this->myClient = new XmlRpcClient(QUrl(QString(this->myUrl.c_str())),0,this->verbose);
        this->myClient->acceptCompressed(false);
    }

    this->errorMessage.clear();

    this->myClient->setDebug(this->verbose);

    QList<QVariant> params;
    QString method = "ping";

    this->connected=false;
    QVariant resp = this->myClient->syncCall(method,params);
    //printf("%0lx::QXmlrpcConnection::connect: resp=%s\n",(unsigned long) QThread::currentThread(), this->variant2string(resp,"\n").toStdString().c_str());
    QString s_resp;
    if(resp.canConvert(QVariant::List) && resp.toList().size()>1) {
        QList<QVariant> lista = resp.toList().at(1).toList();
        //printf("%0lx::QXmlrpcConnection::login: lista=%s\n",(unsigned long) QThread::currentThread(), this->variant2string(lista,"\n").toStdString().c_str());
        if(lista.size()>0) {
            //printf("%0lx::QXmlrpcConnection::login: lista[0]=%s\n",(unsigned long) QThread::currentThread(),this->variant2string(lista.at(0).toByteArray().data(),"\n").toStdString().c_str());
            s_resp = QString( lista.at(0).toByteArray().data() );
            //ret = QXmlrpcConnection::list2resultset( &lista, new QResultSet() );
        } else {
            //this->errorMessage = "Authentication failed!";
        }
    }
    //printf("%0lx::QXmlrpcConnection::connect: s_resp=%s\n",(unsigned long) QThread::currentThread(), s_resp.toStdString().c_str());
    this->connected = s_resp=="pong"; //true;

    //printf("%0x::QXmlrpcConnection::connect: end.\n",(int) QThread::currentThread());
    return true;
}
bool QXmlrpcConnection::disconnect() {
    if(!this->connected)
        return true;
    this->connected=false;
    /*
    if(this->myClient!=0)
        delete this->myClient;
    this->myClient = 0;
    */
    return true;
}
bool QXmlrpcConnection::reconnect() {
    if( this->disconnect() )
        return this->connect();
    else
        return false;
}

bool debugQSqliteExec = false;

ResultSet* QXmlrpcConnection::login(string user, string pwd) {
    //printf("%0x::QXmlrpcConnection::login: start.\n",(int) QThread::currentThread());

    ResultSet* ret=0;

    //printf("%0x::QXmlrpcConnection::login: this->errorMessage.clear()...\n",(int) QThread::currentThread());
    this->errorMessage.clear();

    //printf("%0x::QXmlrpcConnection::login: params...\n",(int) QThread::currentThread());
    QList<QVariant> params;
    QString method = "login";
    params.push_back(QString(user.c_str()));
    params.push_back(QString(pwd.c_str()));

    //printf("%0x::QXmlrpcConnection::login: calling myClient %0x...\n",(int) QThread::currentThread(), (int) this->myClient);
    QVariant resp = this->myClient->syncCall(method,params);
    //printf("%0lx::QXmlrpcConnection::login: resp=%s\n",(unsigned long) QThread::currentThread(), this->variant2string(resp,"\n").toStdString().c_str());

    if(resp.canConvert(QVariant::List) && resp.toList().size()>1) {
        QList<QVariant> lista = resp.toList().at(1).toList();
        if(lista.size()>0) {
            ret = QXmlrpcConnection::list2resultset( &lista, new QResultSet() );
            if(this->verbose) printf("%0lx::QXmlrpcConnection::login: ret=%s\n",(unsigned long) QThread::currentThread(), ret->toString("\n").c_str());
        } else {
            this->errorMessage = "Authentication failed!";
            //printf("%0lx::QXmlrpcConnection::login: lista=%s\n",(unsigned long) QThread::currentThread(), this->variant2string(lista,"\n").toStdString().c_str());
        }
    } else {
        this->errorMessage = "Authentication failed!";
        //printf("%0lx::QXmlrpcConnection::login: %s\n",(unsigned long) QThread::currentThread(), this->variant2string( resp, "\n").toStdString().c_str());
    }
    if(this->errorMessage.length()>0)
        printf("%0lx::QXmlrpcConnection::login: errorMessage=%s\n",(unsigned long) QThread::currentThread(), this->errorMessage.c_str());

    //printf("%0lx::QXmlrpcConnection::login: end.\n",(unsigned long) QThread::currentThread());
    return ret;
}
string QXmlrpcConnection::getFormSchema(string language) {
    string ret;

    this->errorMessage.clear();

    QList<QVariant> params;
    QString method = "getFormSchema";
    params.push_back(QString(language.c_str()));

    QVariant resp = this->myClient->syncCall(method,params);

    if(resp.canConvert(QVariant::List) && resp.toList().size()>0) {
        QString qret = resp.toList().at(0).toString();
        ret = qret.toStdString();
    } else
        printf("QXmlrpcConnection::getFormSchema: %s\n", this->variant2string( resp, "\n").toStdString().c_str());

    return ret;
}
string QXmlrpcConnection::getDBSchema(string language) {
    string ret;

    this->errorMessage.clear();

    QList<QVariant> params;
    QString method = "getDBSchema";
    params.push_back(QString(language.c_str()));

    QVariant resp = this->myClient->syncCall(method,params);

    if(resp.canConvert(QVariant::List) && resp.toList().size()>0) {
        QString qret = resp.toList().at(0).toString();
        ret = qret.toStdString();
    } else
        printf("QXmlrpcConnection::getDBSchema: %s\n", this->variant2string( resp, "\n").toStdString().c_str());

    return ret;
}
string QXmlrpcConnection::getSchemaName() {
    string ret;

    this->errorMessage.clear();

    QList<QVariant> params;
    QString method = "getSchemaName";
    //params.push_back(QString(language.c_str()));

    QVariant resp = this->myClient->syncCall(method,params);

    if(resp.canConvert(QVariant::List) && resp.toList().size()>0) {
        QString qret = resp.toList().at(0).toString();
        ret = qret.toStdString();
    } else
        printf("QXmlrpcConnection::getSchemaName: %s\n", this->variant2string( resp, "\n").toStdString().c_str());

    return ret;
}

ResultSet* QXmlrpcConnection::exec(const string s) {
    QResultSet* rs = new QResultSet();
    this->errorMessage.clear();

    QList<QVariant> params;
    QString method = "selectAsArray";
    params.push_back(QString("nometabella"));
    params.push_back(QString(s.c_str()));

    QVariant v = this->myClient->syncCall(method,params);
    //if(this->verbose) printf("QXmlrpcConnection::exec: %s\n", this->variant2string( v, "\n").toStdString().c_str());

    if(!v.canConvert(QVariant::List)) {
        this->errorMessage.append( "Server returned a wrong type : "
            + string( ((unsigned long)v.typeName())!=0 ? v.typeName() : "null")
            + " instead of QVariant::List"
            );
        printf("QXmlrpcConnection::exec: v.typeName=%s\n",v.typeName());
        //this->errorMessage.append(string(v.typeName()));
        //this->errorMessage.append(" instead of QVariant::List");
    } else if(v.toList().size()==0) {
        this->errorMessage.append( "Server returned an empty list." );
    } else if(v.toList().at(0).type()!=QVariant::ByteArray && v.toList().at(0).type()!=QVariant::String) {
        this->errorMessage.append( "Server returned a wrong type at return[0]: " + string(v.toList().at(0).typeName())
            + " instead of QVariant::ByteArray"
            + " or QVariant::String" );
    } else if(v.toList().at(1).type()!=QVariant::List) {
        this->errorMessage.append( "Server returned a wrong type at return[1]: " + string(v.toList().at(1).typeName())
            + " instead of QVariant::List");
    } else {
        //if(this->verbose) printf("QXmlrpcConnection::exec: %s\n", v.toList().at(0).toString().toStdString().c_str());
        QList<QVariant> lista = v.toList().at(1).toList();
        rs = QXmlrpcConnection::list2resultset( &lista, rs );
    }
    return rs;
}
string QXmlrpcConnection::escapeString(string s) const {
    static string fromQuote("\'");
    static string   toQuote("\'\'");
    return DBLayer::replaceAll(s, fromQuote, toQuote);
}
int QXmlrpcConnection::getColumnSize(string* relname) {
    int ret = -1;
    this->errorMessage.clear();

    QList<QVariant> params;
    QString method = "getColumnSize";
    params.push_back(QString(relname->c_str()));

    QVariant resp = this->myClient->syncCall(method,params);

    if(resp.canConvert(QVariant::List) && resp.toList().size()>1) {
        ret = resp.toList().at(1).toList().at(0).toInt();
    } else
        printf("QXmlrpcConnection::getColumnSize: %s\n", this->variant2string( resp, "\n").toStdString().c_str());

    return ret;
}
string QXmlrpcConnection::getColumnName(string* relname, int column) {
    string ret="";
    this->errorMessage.clear();

    QList<QVariant> params;
    QString method = "getColumnName";
    params.push_back(QString(relname->c_str()));
    params.push_back(column);

    QVariant resp = this->myClient->syncCall(method,params);

    if(resp.canConvert(QVariant::List) && resp.toList().size()>1) {
        ret = resp.toList().at(1).toList().at(0).toString().toStdString();
    } else
        printf("QXmlrpcConnection::getColumnName: %s\n", this->variant2string( resp, "\n").toStdString().c_str());

    return ret;
}
DBLayer::IntegerVector QXmlrpcConnection::getKeys(string* relname) {
    IntegerVector ret;
    this->errorMessage.clear();

    QList<QVariant> params;
    QString method = "getKeys";
    params.push_back(QString(relname->c_str()));

    QVariant resp = this->myClient->syncCall(method,params);

    if(this->verbose) printf("QXmlrpcConnection::getKeys: resp=%s\n", this->variant2string( resp, "\n").toStdString().c_str());
    if(resp.canConvert(QVariant::List) && resp.toList().size()>1) {
        for(int i=0; i<resp.toList().at(1).toList().size(); i++) {
           ret.push_back( resp.toList().at(1).toList().at(i).toInt() );
        }
    } else
        printf("QXmlrpcConnection::getKeys: %s\n", this->variant2string( resp, "\n").toStdString().c_str());

    return ret;
}
DBLayer::IntegerVector QXmlrpcConnection::getForeignKeys(string* relname) {
    IntegerVector ret;
    this->errorMessage.clear();

    QList<QVariant> params;
    QString method = "getForeignKeys";
    params.push_back(QString(relname->c_str()));

    QVariant resp = this->myClient->syncCall(method,params);

    if(resp.canConvert(QVariant::List) && resp.toList().size()>1) {
        for(int i=0; i<resp.toList().at(1).toList().size(); i++) {
           ret.push_back( resp.toList().at(1).toList().at(i).toInt() );
        }
    } else
        printf("QXmlrpcConnection::getForeignKeys: %s\n", this->variant2string( resp, "\n").toStdString().c_str());

    return ret;
}
ColumnDefinitions QXmlrpcConnection::getColumnsForTable(const string& tablename) {
    ColumnDefinitions ret;
    this->errorMessage.clear();

    QList<QVariant> params;
    QString method = "getColumnsForTable";
    params.push_back(QString(tablename.c_str()));

    QVariant resp = this->myClient->syncCall(method,params);

    if(this->verbose) printf("QXmlrpcConnection::getColumnsForTable: resp=%s\n", this->variant2string( resp, "\n").toStdString().c_str());
    if(resp.canConvert(QVariant::List) && resp.toList().size()>1) {
        QMap<QString,QVariant> mymap = resp.toList().at(1).toMap();
        //printf("QXmlrpcConnection::getColumnsForTable: mymap=%s\n", this->variant2string( mymap, "\n").toStdString().c_str());
        // nome, tipo, nullo, hasdefault, valoredefault
        for(const QString& k : mymap.keys()) {
            StringVector row;
            QMap<QString,QVariant> mysubmap = mymap[k].toMap();
            row.push_back(mysubmap["Field"].toString().toStdString());
            row.push_back(mysubmap["Type"].toString().toStdString());
            row.push_back(mysubmap["Null"].toString().toStdString());
            if(mysubmap.contains(QString("Default"))) {
                row.push_back(mysubmap["Default"].toString().toStdString());
            } else {
                row.push_back("\\N");
            }
            ret[ k.toStdString() ] = row;
        }
    } else
        printf("QXmlrpcConnection::getColumnsForTable: %s\n", this->variant2string( resp, "\n").toStdString().c_str());
    return ret;
}

QString QXmlrpcConnection::variant2string(const QVariant& v, QString prefix) {
    QString ret;
    QStringList l;
    QList<QString> chiavi;
    switch( v.type() ) {
        case QVariant::ByteArray:
            ret = prefix + QString("<value type=\"") + QString(v.typeName()) + QString("\" bytes=\"") + QString("%1").arg(v.toByteArray().size()) + QString("\">") + v.toString() + QString("</value>");
            break;
        case QVariant::List:
            l.clear();
            for(int i=0; i<v.toList().size(); i++)
                l.append( variant2string(v.toList().at(i), prefix+QString("  ")) );
            ret = prefix + QString("<list>")
                  + l.join(QString())
                  + prefix + QString("</list>");
            break;
        case QVariant::Map:
            chiavi.clear();
            chiavi = v.toMap().keys();
            for(int i=0; i<chiavi.size(); i++)
                l.append( prefix+QString("  ")+QString("<key>")+chiavi.at(i)+QString("</key>")
                          +prefix+QString("  ")+QString("<value>")
                          +variant2string(v.toMap().value(chiavi.at(i)), prefix+QString("  "))
                          +prefix+QString("  ")+QString("</value>")
                          );
            ret = prefix+QString("<map>")
                  + l.join(QString())
                  + prefix+QString("</map>");
            break;
        default:
            ret = prefix + QString("<value type=\"") + QString(v.typeName()) + QString("\">") + v.toString() + QString("</value>");
            break;
    }
    return ret;
}
QResultSet* QXmlrpcConnection::list2resultset(QList<QVariant>* iLista, QResultSet* ioResultSet) {
    QStringList l;
    QList<QString> chiavi;
    l.clear();
    for(int l=0; l<iLista->size(); l++) {
        QMap<QString,QVariant> map = iLista->at(l).toMap();
        chiavi.clear();
        chiavi = map.keys();
        unsigned int colonna=0;
        for(int i=0; i<chiavi.size(); i++) {
            // Metadati
            if(ioResultSet->columnName.size()<=colonna) {
                ioResultSet->columnName.push_back( chiavi.at(i).toStdString() );
            }
            switch(map[chiavi.at(i)].type()) {
                case QVariant::Int:
                case QVariant::LongLong:
                case QVariant::UInt:
                case QVariant::ULongLong:
                    if(ioResultSet->columnType.size()<=colonna) ioResultSet->columnType.push_back( DBLayer::type_integer );
                    ioResultSet->righe.push_back( map[chiavi.at(i)].toString().toStdString() );
                    break;
                case QVariant::Double:
                    if(ioResultSet->columnType.size()<=colonna) ioResultSet->columnType.push_back( DBLayer::type_integer );
                    ioResultSet->righe.push_back( map[chiavi.at(i)].toString().toStdString() );
                    break;
                case QVariant::Bool:
                    if(ioResultSet->columnType.size()<=colonna) ioResultSet->columnType.push_back( DBLayer::type_boolean );
                    ioResultSet->righe.push_back( map[chiavi.at(i)].toBool() ? string("1") : string("0") );
                    break;

                case QVariant::String:
                    if(ioResultSet->columnType.size()<=colonna) ioResultSet->columnType.push_back( DBLayer::type_string );
                    ioResultSet->righe.push_back( map[chiavi.at(i)].toString().toStdString() );
                    break;
                case QVariant::ByteArray:
                    if(ioResultSet->columnType.size()<=colonna) ioResultSet->columnType.push_back( DBLayer::type_string );
                    ioResultSet->righe.push_back( string( map[chiavi.at(i)].toByteArray().data() ) );
                    break;

                case QVariant::Date:
                case QVariant::DateTime:
                    if(ioResultSet->columnType.size()<=colonna) ioResultSet->columnType.push_back( DBLayer::type_datetime );
                    ioResultSet->righe.push_back( string( map[chiavi.at(i)].toByteArray().data() ) );

                default:
                    if(ioResultSet->columnType.size()<=colonna) ioResultSet->columnType.push_back( DBLayer::type_blob );
                    ioResultSet->righe.push_back( map[chiavi.at(i)].toString().toStdString() + ( QString(" (%1)").arg(map[chiavi.at(i)].typeName()) ).toStdString() );
                    break;
            }
            colonna++;
        }
    }
    return ioResultSet;
}

// **************** Proxy Connections *********************
// The proxy connections are used by DBMgr to execute the following methods
bool QXmlrpcConnection::isProxy() const { return true; }
DBEntity* QXmlrpcConnection::Insert(DBEntity *dbe) {
    //printf("%0x::QXmlrpcConnection::Insert: start.\n",(int) QThread::currentThread());
    this->errorMessage.clear();

    QList<QVariant> v;
    this->_dbeToVariant(dbe,&v);

    QList<QVariant> params;
    params.push_back(v);
    QString method = "insert";

    QVariant resp = this->myClient->syncCall(method,params);
    if(resp.canConvert(QVariant::List) && resp.toList().size()>1
       && resp.toList().at(1).canConvert(QVariant::List) && resp.toList().at(1).toList().size()>0) {
        dbe = this->_variantToDBE( (QVariant*) &(resp.toList().at(1).toList().at(0)), dbe);
    } else {
        printf("QXmlrpcConnection::Insert: v=%s\n", this->variant2string( v, "\n").toStdString().c_str());
        printf("QXmlrpcConnection::Insert: resp=%s\n", this->variant2string( resp, "\n").toStdString().c_str());
    }
    //printf("%0x::QXmlrpcConnection::Insert: end.\n",(int) QThread::currentThread());
    return dbe;
}
DBEntity* QXmlrpcConnection::Update(DBEntity *dbe) {
    this->errorMessage.clear();

    QList<QVariant> v;
    this->_dbeToVariant(dbe,&v);

    QList<QVariant> params;
    params.push_back(v);
    QString method = "update";

    QVariant resp = this->myClient->syncCall(method,params);
    if(resp.canConvert(QVariant::List) && resp.toList().size()>1
       && resp.toList().at(1).canConvert(QVariant::List) && resp.toList().at(1).toList().size()>0) {
        dbe = this->_variantToDBE( (QVariant*) &(resp.toList().at(1).toList().at(0)), dbe);
    } else
        printf("QXmlrpcConnection::Update: %s\n", this->variant2string( resp, "\n").toStdString().c_str());

    return dbe;
}
DBEntity* QXmlrpcConnection::Delete(DBEntity *dbe) {
    this->errorMessage.clear();

    QList<QVariant> v;
    this->_dbeToVariant(dbe,&v);

    QList<QVariant> params;
    params.push_back(v);
    QString method = "delete";

    QVariant resp = this->myClient->syncCall(method,params);
    if(resp.canConvert(QVariant::List) && resp.toList().size()>1
       && resp.toList().at(1).canConvert(QVariant::List) && resp.toList().at(1).toList().size()>0) {
        dbe = this->_variantToDBE( (QVariant*) &(resp.toList().at(1).toList().at(0)), dbe);
    } else
        printf("QXmlrpcConnection::Delete: %s\n", this->variant2string( resp, "\n").toStdString().c_str());

    return dbe;
}
DBEntityVector* QXmlrpcConnection::Select(DBEntity* dbe, const string& tableName, const string& searchString) {
    this->errorMessage.clear();

    QList<QVariant> params;
    params.push_back(QString(tableName.c_str()));
    params.push_back(QString(searchString.c_str()));
    QString method = "select";

    QVariant resp = this->myClient->syncCall(method,params);
    DBEntityVector* ret = new DBEntityVector;

    if(this->verbose) printf("QXmlrpcConnection::Select: resp=%s\n", this->variant2string( resp, "\n").toStdString().c_str());
    if(resp.canConvert(QVariant::List) && resp.toList().size()>1) {
        if(this->verbose) printf("QXmlrpcConnection::Select: %s\n", resp.toList().at(0).toString().toStdString().c_str());
        QList<QVariant> lista = resp.toList().at(1).toList(); //.at(0).toList();
        for(int i=0; i<lista.size(); i++) {
            ret->push_back( this->_variantToDBE( &(lista[i]), dbe->createNewInstance() ) );
        }
    } else
        printf("QXmlrpcConnection::Select: cannot convert this %s\n", this->variant2string( resp, "\n").toStdString().c_str());

    return ret;
}
DBEntityVector* QXmlrpcConnection::Search(DBEntity* dbe, bool uselike, bool caseSensitive, const string& orderBy ) {
    //printf("%0x::QXmlrpcConnection::Search: start.\n",(int) QThread::currentThread());
    this->errorMessage.clear();
    QList<QVariant> cerca;
    this->_dbeToVariant(dbe,&cerca);

    QList<QVariant> params;
    params.push_back(cerca);
    params.push_back(uselike);
    params.push_back(caseSensitive);
    params.push_back(QString(orderBy.c_str()));
    QString method = "search";

    //printf("%0x::QXmlrpcConnection::Search: invoking remote method...\n",(int) QThread::currentThread());
    QVariant resp = this->myClient->syncCall(method,params);
    DBEntityVector* ret = new DBEntityVector;

    if(resp.canConvert(QVariant::List) && resp.toList().size()>1) {
        QList<QVariant> lista = resp.toList().at(1).toList();
        for(int i=0; i<lista.size(); i++) {
            ret->push_back( this->_variantToDBE( &(lista[i]), dbe->createNewInstance() ) );
        }
    } else
        printf("%0lx::QXmlrpcConnection::Search: %s\n",(unsigned long) QThread::currentThread(), this->variant2string( resp, "\n").toStdString().c_str());

    //printf("%0x::QXmlrpcConnection::Search: end.\n",(int) QThread::currentThread());
    return ret;
}
string QXmlrpcConnection::ping() {
    this->errorMessage.clear();

    this->myClient->setDebug(this->verbose);

    QList<QVariant> params;
    QString method = "ping";

    this->connected=false;
    QVariant resp = this->myClient->syncCall(method,params);
    //printf("%0lx::QXmlrpcConnection::connect: resp=%s\n",(unsigned long) QThread::currentThread(), this->variant2string(resp,"\n").toStdString().c_str());
    QString ret;
    if(resp.canConvert(QVariant::List) && resp.toList().size()>1) {
        QList<QVariant> lista = resp.toList().at(1).toList();
        //printf("%0lx::QXmlrpcConnection::login: lista=%s\n",(unsigned long) QThread::currentThread(), this->variant2string(lista,"\n").toStdString().c_str());
        if(lista.size()>0) {
            //printf("%0lx::QXmlrpcConnection::login: lista[0]=%s\n",(unsigned long) QThread::currentThread(),this->variant2string(lista.at(0).toByteArray().data(),"\n").toStdString().c_str());
            ret = QString( lista.at(0).toByteArray().data() );
            //ret = QXmlrpcConnection::list2resultset( &lista, new QResultSet() );
        } else {
            //this->errorMessage = "Authentication failed!";
        }
    }
    //printf("%0lx::QXmlrpcConnection::connect: s_resp=%s\n",(unsigned long) QThread::currentThread(), s_resp.toStdString().c_str());
    return ret.toStdString();
}

QList<QVariant>* QXmlrpcConnection::_dbeToVariant(DBEntity* dbe, QList<QVariant>* ioVariant) {
    QMap<QString,QVariant>* d = new QMap<QString,QVariant>();
    ioVariant->push_back( QVariant(dbe->name().c_str()) );
    StringVector names = dbe->getNames();
    for(StringVector::iterator it=names.begin(); it!=names.end(); it++) {
        Field* field = dbe->getField(*it);
        //printf("QXmlrpcConnection::_dbeToVariant: %s type=%d  - is %s null\n", (*it).c_str(), field->getType(), field->isNull()?"":"not");
        if(field->isNull()) continue;
        //if(field->getStringValue()!=0) printf("QXmlrpcConnection::_dbeToVariant: %s=>%s\n", (*it).c_str(), field->getStringValue()->c_str());
        if(field->isBoolean()) d->insert(QString( (*it).c_str() ), field->getBooleanValue());
        else if(field->isInteger()) d->insert(QString( (*it).c_str() ), (qlonglong) field->getIntegerValue());
        else if(field->isFloat()) d->insert(QString( (*it).c_str() ), field->getFloatValue());
        else if(field->isDouble()) d->insert(QString( (*it).c_str() ), field->getFloatValue());
        else if(field->isDate()) {
            //printf("QXmlrpcConnection::_dbeToVariant: %s=>%s\n", (*it).c_str(), field->toString().c_str());
            d->insert(QString( (*it).c_str() ), field->toString().c_str());
        } else d->insert(QString( (*it).c_str() ), field->getStringValue()->c_str());
    }
    ioVariant->push_back( (*d) );
    return ioVariant;
}
DBEntity* QXmlrpcConnection::_variantToDBE(QVariant* v, DBEntity* ioDbe) {
    QString typeName = v->toMap()["_typeName"].toString();
    //printf("QXmlrpcConnection::_variantToDBE: ioDbe->name()=%s\n", ioDbe->name().c_str());
    //printf("QXmlrpcConnection::_variantToDBE: typeName=%s\n", typeName.toStdString().c_str());
    // NOTE se il tipo tornato non coincide => abortisce :-( questo causava 34 entry vuote x l'auto fill di mylog
    if( strcmp(ioDbe->name().c_str(),typeName.toStdString() .c_str())!=0 ) {
        return ioDbe;
    }
    QList<QString> chiavi = v->toMap().keys();
    for(int i=0; i<chiavi.size(); i++) {
        QString qkey = chiavi[i];
        if(qkey=="_typeName" || qkey=="_typename") continue;
        string key = qkey.toStdString();
        string value = v->toMap()[ qkey ].toString().toStdString();
        switch(v->toMap()[ qkey ].type()) {
            case QVariant::Bool:
                ioDbe->setValue(key, (bool) v->toMap()[ qkey ].toBool());
                break;
            case QVariant::Int:
            case QVariant::LongLong:
            case QVariant::UInt:
            case QVariant::ULongLong:
                ioDbe->setValue(key, (long) v->toMap()[ qkey ].toLongLong());
                break;
            case QVariant::Double:
                ioDbe->setValue(key, (float) v->toMap()[ qkey ].toDouble());
                break;
            case QVariant::Date:
            case QVariant::DateTime:
            case QVariant::Time:
                ioDbe->setDateValue(key, value);
                break;
            default:
                ioDbe->setValue(key, value);
                break;
        }
        //printf("QXmlrpcConnection::_variantToDBE: %s=>%s (%d::%s)\n", key.c_str(), value.c_str(), v->toMap()[ qkey ].type(), v->toMap()[ qkey ].typeName());
    }
    return ioDbe;
}
// **************** Proxy Connections: end. *********************


// **************************************** SLOTS ****************************************
