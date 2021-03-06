#include <cstdio>

#include <QtSql/QSqlError>
#include <QtSql/QSqlField>
#include <QtSql/QSqlIndex>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>

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
    if(this->verbose) {
        for(const QString name : QSqlDatabase::connectionNames()) {
            cout << "name: " << name.toStdString() << endl;
        }
    }
    if(!QSqlDatabase::contains("dblayer_qsqlite")) {
        db = QSqlDatabase::addDatabase("QSQLITE","dblayer_qsqlite");
    } else {
        db = QSqlDatabase::database("dblayer_qsqlite");
    }
    if(this->verbose) cout << "QSqliteConnection::QSqliteConnection: db.connectionName()=" << db.connectionName().toStdString() << endl;
//     cout << "QSqliteConnection::QSqliteConnection: defaultConnection=" << QSqlDatabase::defaultConnection << endl;
}
QSqliteConnection::~QSqliteConnection() {
    this->disconnect();
//     this->db.removeDatabase("dblayer_qsqlite");
    
    // NOTE I guess this should be done automatically when the object is destroyed
//     for(const QString name : QSqlDatabase::connectionNames()) {
//         cout << "QSqliteConnection::~QSqliteConnection: removing " << name.toStdString() << endl;
//         QSqlDatabase::removeDatabase(name);
//     }
}

bool QSqliteConnection::connect() {
    if(this->verbose) printf("QSqliteConnection::connect: start.\n");
    this->errorMessage.clear();
    this->connected = true;
    if(this->verbose) cout << "QSqliteConnection::connect: dbname=" << this->dbname << endl;
    db.setDatabaseName( QString(this->dbname.c_str()) );
    if(!db.open()) {
        this->connected = false;
        this->errorMessage.append("Unable to open '").append(this->dbname.c_str()).append("'.");
    }
    if(this->verbose) printf("QSqliteConnection::connect: end.\n");
    return this->connected;
}
bool QSqliteConnection::disconnect() {
    if(this->verbose) printf("QSqliteConnection::disconnect: start.\n");
    if(!this->connected) {
        if(this->verbose) printf("QSqliteConnection::disconnect: end2.\n");
        return true;
    }
    this->db.close();
    this->connected=false;
    if(this->verbose) printf("QSqliteConnection::disconnect: end.\n");
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
    if(this->verbose) printf("QSqliteConnection::exec: start.\n");
    QResultSet* rs = new QResultSet();
    this->errorMessage.clear();

    QSqlQuery query(db);
    query.exec(QString(s.c_str()));

    if(query.lastError().text().trimmed().length()>0) {
        this->errorMessage.append( query.lastError().text().trimmed().toStdString() );
        if(this->verbose) cerr << "QSqliteConnection::exec: errorMessage=" << errorMessage << endl;
        if(this->verbose) cerr << "QSqliteConnection::exec: s=" << s << endl;
    }
    QSqlRecord record = query.record();

    // Preparing metadata
    int nColonne = record.count();
    bool unknown_type = false;
    for( int i=0; i<nColonne; i++) {
        QVariant::Type fieldType = record.field(i).type();
        bool add_column = true;
        switch(fieldType) {
            case QVariant::Int:
            case QVariant::LongLong:
                rs->columnType.push_back( DBLayer::type_integer );
                break;
            case QVariant::String:
            case QVariant::Char:
                rs->columnType.push_back( DBLayer::type_string );
                break;
            case QVariant::Double:
                rs->columnType.push_back( DBLayer::type_double );
                break;
            case QVariant::Bool:
                rs->columnType.push_back( DBLayer::type_boolean );
                break;
            case QVariant::Date:
            case QVariant::DateTime:
                rs->columnType.push_back( DBLayer::type_datetime );
                break;
            case 0:
                // If the type is 0 means that the column is empty for all the rows
                //cout << "QSqliteConnection::exec: type=0 for column " << record.fieldName(i).toStdString() << endl;
                // Add it anyway, otherwise we have wrong results
                //add_column = false;
                rs->columnType.push_back( DBLayer::type_blob );
                //unknown_type = true;
                break;
            default:
                cerr << "QSqliteConnection::exec: \'" << record.fieldName(i).toStdString() << "\' "
                     << "fieldType \'" << fieldType << "\' Unknown!" << endl;
                rs->columnType.push_back( DBLayer::type_blob );
                unknown_type = true;
                break;
        }
        if(add_column) {
            rs->columnName.push_back(record.fieldName(i).toStdString());
            rs->columnSize.push_back(record.field(i).length());
        }
    }

    // Fetching rows
    while(query.next()) {
        for(int i = 0; i < record.count(); i++) {
            QVariant val =  query.value(i);
            rs->righe.push_back( val.toString().toStdString() );
            if(unknown_type) {
                cout << rs->columnName.at(i) << "::" << rs->columnType.at(i) << "::" << val.toString().toStdString() << endl;
            }
        }
    }
    if(this->verbose) printf("QSqliteConnection::exec: end.\n");
    return rs;
}
string QSqliteConnection::escapeString(string s) const {
    static string fromQuote("\'");
    static string   toQuote("\'\'");
    return DBLayer::replaceAll(s, fromQuote, toQuote);
}
string QSqliteConnection::quoteDate(string s) const { return "'"+s+"'"; }
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
// DBLayer::IntegerVector QSqliteConnection::getKeys(string* relname) {
//     IntegerVector ret;
//     this->errorMessage.clear();
// 
//     if(!db.open()) {
//         this->connected = false;
//         this->errorMessage.append("Unable to open '").append(this->dbname.c_str()).append("'.");
//         return ret;
//     }
// 
//     printf("QSqliteConnection::getKeys: TODO\n");
//     QSqlIndex index = db.primaryIndex(QString(relname->c_str()));
//     cout << "QSqliteConnection::getKeys: index.name()=" << index.name().toStdString() << endl;
//     QVariant resp;// = this->myClient->syncCall(method,params);
// 
//     if(this->verbose) printf("QSqliteConnection::getKeys: resp=%s\n", this->variant2string( resp, "\n").toStdString().c_str());
//     if(resp.canConvert(QVariant::List) && resp.toList().size()>1) {
//         for(int i=0; i<resp.toList().at(1).toList().size(); i++) {
//            ret.push_back( resp.toList().at(1).toList().at(i).toInt() );
//         }
//     } else
//         printf("QSqliteConnection::getKeys: %s\n", this->variant2string( resp, "\n").toStdString().c_str());
// 
//     return ret;
// }
// DBLayer::IntegerVector QSqliteConnection::getForeignKeys(string* relname) {
//     IntegerVector ret;
//     this->errorMessage.clear();
// 
//     printf("QSqliteConnection::getForeignKeys: TODO\n");
//     QVariant resp;// = this->myClient->syncCall(method,params);
// 
//     if(resp.canConvert(QVariant::List) && resp.toList().size()>1) {
//         for(int i=0; i<resp.toList().at(1).toList().size(); i++) {
//            ret.push_back( resp.toList().at(1).toList().at(i).toInt() );
//         }
//     } else
//         printf("QSqliteConnection::getForeignKeys: %s\n", this->variant2string( resp, "\n").toStdString().c_str());
// 
//     return ret;
// }
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
                // Column Name
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
                    break;
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
