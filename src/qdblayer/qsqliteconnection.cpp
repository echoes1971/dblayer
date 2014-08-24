#include <cstdio>

#include <QtSql/QSqlQuery>

#include "qsqliteconnection.h"
#include "qresultset.h"
using namespace DBLayer;


Connection* DBLayer::createQSqliteConnection(string s) {
    return new QSqliteConnection(s);
}
void QSqliteConnection::registerClass() {
    //printf("DBLayer::__init_qxmlrpc: start.\n");
    DBLayer::registerConnectionType("qsqlite",&DBLayer::createQSqliteConnection);
    //printf("DBLayer::__init_qxmlrpc: end.\n");
}

#ifdef WIN32
QSqliteConnection::QSqliteConnection(string s) : Connection(s) {
//QSqliteConnection::QSqliteConnection(string s, QObject* parent) : QObject(parent), Connection(s) {
#else
QSqliteConnection::QSqliteConnection(string s) : Connection::Connection(s) {
//QSqliteConnection::QSqliteConnection(string s, QObject* parent) : QObject(parent), Connection::Connection(s) {
#endif
    this->dbname = s;
    db = QSqlDatabase::addDatabase("QSQLITE");
}
QSqliteConnection::~QSqliteConnection() {
    this->disconnect();
}

bool QSqliteConnection::connect() {
    printf("QSqliteConnection::connect: start.\n");
    this->errorMessage.clear();
    this->connected = true;
    cout << "QSqliteConnection::connect: dbname=" << this->dbname << endl;
    db.setDatabaseName( QString(this->dbname.c_str()) );
    if(!db.open()) {
        this->connected = false;
        this->errorMessage.append("Unable to open '").append(this->dbname.c_str()).append("'.");
    }
    printf("QSqliteConnection::connect: end.\n");
    return this->connected;
}
bool QSqliteConnection::disconnect() {
    printf("QSqliteConnection::disconnect: start.\n");
    if(!this->connected)
        return true;
    db.close();
    this->connected=false;
    printf("QSqliteConnection::disconnect: end.\n");
    return true;
}
bool QSqliteConnection::reconnect() {
    if( this->disconnect() )
        return this->connect();
    else
        return false;
}

bool debugQSqliteExec = false;


ResultSet* QSqliteConnection::exec(const string s) {
    printf("QSqliteConnection::exec: start.\n");
    QResultSet* rs = new QResultSet();
    this->errorMessage.clear();

    printf("QSqliteConnection::exec: TODO\n");
    QSqlQuery query(db);
    query.exec(QString(s.c_str()));

    DA QUI


//    QList<QVariant> lista = v.toList().at(1).toList();
//    rs = QSqliteConnection::list2resultset( &lista, rs );
    return rs;
}
string QSqliteConnection::escapeString(string s) const {
    static string fromQuote("\'");
    static string   toQuote("\'\'");
    return DBLayer::replaceAll(s, fromQuote, toQuote);
}
int QSqliteConnection::getColumnSize(string* relname) {
    int ret = -1;
    this->errorMessage.clear();

    printf("QSqliteConnection::getColumnSize: TODO\n");
    QVariant resp;// = this->myClient->syncCall(method,params);

    if(resp.canConvert(QVariant::List) && resp.toList().size()>1) {
        ret = resp.toList().at(1).toList().at(0).toInt();
    } else
        printf("QSqliteConnection::getColumnSize: %s\n", this->variant2string( resp, "\n").toStdString().c_str());

    return ret;
}
string QSqliteConnection::getColumnName(string* relname, int column) {
    string ret="";
    this->errorMessage.clear();

    printf("QSqliteConnection::getColumnName: TODO\n");
    QVariant resp;// = this->myClient->syncCall(method,params);

    if(resp.canConvert(QVariant::List) && resp.toList().size()>1) {
        ret = resp.toList().at(1).toList().at(0).toString().toStdString();
    } else
        printf("QSqliteConnection::getColumnName: %s\n", this->variant2string( resp, "\n").toStdString().c_str());

    return ret;
}
DBLayer::IntegerVector QSqliteConnection::getKeys(string* relname) {
    IntegerVector ret;
    this->errorMessage.clear();

    printf("QSqliteConnection::getKeys: TODO\n");
    QVariant resp;// = this->myClient->syncCall(method,params);

    if(this->verbose) printf("QSqliteConnection::getKeys: resp=%s\n", this->variant2string( resp, "\n").toStdString().c_str());
    if(resp.canConvert(QVariant::List) && resp.toList().size()>1) {
        for(int i=0; i<resp.toList().at(1).toList().size(); i++) {
           ret.push_back( resp.toList().at(1).toList().at(i).toInt() );
        }
    } else
        printf("QSqliteConnection::getKeys: %s\n", this->variant2string( resp, "\n").toStdString().c_str());

    return ret;
}
DBLayer::IntegerVector QSqliteConnection::getForeignKeys(string* relname) {
    IntegerVector ret;
    this->errorMessage.clear();

    printf("QSqliteConnection::getForeignKeys: TODO\n");
    QVariant resp;// = this->myClient->syncCall(method,params);

    if(resp.canConvert(QVariant::List) && resp.toList().size()>1) {
        for(int i=0; i<resp.toList().at(1).toList().size(); i++) {
           ret.push_back( resp.toList().at(1).toList().at(i).toInt() );
        }
    } else
        printf("QSqliteConnection::getForeignKeys: %s\n", this->variant2string( resp, "\n").toStdString().c_str());

    return ret;
}
ColumnDefinitions QSqliteConnection::getColumnsForTable(const string& tablename) {
    ColumnDefinitions ret;
    this->errorMessage.clear();

    printf("QSqliteConnection::getColumnsForTable: TODO\n");
    QVariant resp; // = this->myClient->syncCall(method,params);

    if(this->verbose) printf("QSqliteConnection::getColumnsForTable: resp=%s\n", this->variant2string( resp, "\n").toStdString().c_str());
    if(resp.canConvert(QVariant::List) && resp.toList().size()>1) {
        QMap<QString,QVariant> mymap = resp.toList().at(1).toMap();
        //printf("QSqliteConnection::getColumnsForTable: mymap=%s\n", this->variant2string( mymap, "\n").toStdString().c_str());
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
        printf("QSqliteConnection::getColumnsForTable: %s\n", this->variant2string( resp, "\n").toStdString().c_str());
    return ret;
}

QString QSqliteConnection::variant2string(const QVariant& v, QString prefix) {
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
QResultSet* QSqliteConnection::list2resultset(QList<QVariant>* iLista, QResultSet* ioResultSet) {
    QList<QString> chiavi;
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


QList<QVariant>* QSqliteConnection::_dbeToVariant(DBEntity* dbe, QList<QVariant>* ioVariant) {
    QMap<QString,QVariant>* d = new QMap<QString,QVariant>();
    ioVariant->push_back( QVariant(dbe->name().c_str()) );
    StringVector names = dbe->getNames();
    for(StringVector::iterator it=names.begin(); it!=names.end(); it++) {
        Field* field = dbe->getField(*it);
        //printf("QSqliteConnection::_dbeToVariant: %s type=%d  - is %s null\n", (*it).c_str(), field->getType(), field->isNull()?"":"not");
        if(field->isNull()) continue;
        //if(field->getStringValue()!=0) printf("QSqliteConnection::_dbeToVariant: %s=>%s\n", (*it).c_str(), field->getStringValue()->c_str());
        if(field->isBoolean()) d->insert(QString( (*it).c_str() ), field->getBooleanValue());
        else if(field->isInteger()) d->insert(QString( (*it).c_str() ), (qlonglong) field->getIntegerValue());
        else if(field->isFloat()) d->insert(QString( (*it).c_str() ), field->getFloatValue());
        else if(field->isDouble()) d->insert(QString( (*it).c_str() ), field->getFloatValue());
        else if(field->isDate()) {
            //printf("QSqliteConnection::_dbeToVariant: %s=>%s\n", (*it).c_str(), field->toString().c_str());
            d->insert(QString( (*it).c_str() ), field->toString().c_str());
        } else d->insert(QString( (*it).c_str() ), field->getStringValue()->c_str());
    }
    ioVariant->push_back( (*d) );
    return ioVariant;
}
DBEntity* QSqliteConnection::_variantToDBE(QVariant* v, DBEntity* ioDbe) {
    QString typeName = v->toMap()["_typeName"].toString();
    //printf("QSqliteConnection::_variantToDBE: ioDbe->name()=%s\n", ioDbe->name().c_str());
    //printf("QSqliteConnection::_variantToDBE: typeName=%s\n", typeName.toStdString().c_str());
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
        //printf("QSqliteConnection::_variantToDBE: %s=>%s (%d::%s)\n", key.c_str(), value.c_str(), v->toMap()[ qkey ].type(), v->toMap()[ qkey ].typeName());
    }
    return ioDbe;
}
// **************** Proxy Connections: end. *********************


// **************************************** SLOTS ****************************************
