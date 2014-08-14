/**
 * dbschema.cpp
 */

#include "cmschema.h"
using namespace CMSchema;

#include <string>
using namespace std;

//*********************** DBELog: start.
const string DBELog::nomiCampiChiave[] = { string("ip"), string("data") };
ColumnDefinitions DBELog::_columns;
ColumnDefinitions DBELog::getColumns() const { return DBELog::_columns; }
StringField DBELog::chiave1( DBELog::nomiCampiChiave[0] );
StringField DBELog::chiave2( DBELog::nomiCampiChiave[1] );
DBFieldVector DBELog::chiavi = DBELog::___init_keys();
DBFieldVector DBELog::___init_keys() { DBFieldVector ret = DBFieldVector(); ret.push_back( &DBELog::chiave1 ); ret.push_back( &DBELog::chiave2 ); return ret; }
DBELog::DBELog() {
    this->tableName.clear();
    this->schemaName = CMSchema::getSchema();
    if(DBELog::_columns.size()==0) {
        for(const pair<string,vector<string> > pair: DBEntity::getColumns()) {
            DBELog::_columns[pair.first] = pair.second;
        }
        DBELog::_columns["ip"] = vector<string> {"uuid","not null"};
        DBELog::_columns["data"] = vector<string> {"date","not null default '0000-00-00'"};
        DBELog::_columns["ora"] = vector<string> {"time","not null default '00:00:00'"};
        DBELog::_columns["count"] = vector<string> {"int","not null default 0"};
        DBELog::_columns["url"] = vector<string> {"varchar(255)","default null"};
        DBELog::_columns["note"] = vector<string> {"varchar(255)","not null default ''"};
        DBELog::_columns["note2"] = vector<string> {"text","not null"};
    }
}
DBELog::~DBELog() {}
string DBELog::name() const { return "DBELog"; }
string DBELog::getTableName() const { return "log"; }
DBFieldVector* DBELog::getKeys() const { return &DBELog::chiavi; }
DBELog* DBELog::createNewInstance() const { return new DBELog(); }
DBLayer::StringVector DBELog::getOrderBy() const {
    static DBLayer::StringVector ret({"data desc", "ora desc"});
    return ret;
}
//*********************** DBELog: end.

//*********************** DBEObject: start.
const string DBEObject::nomiCampiChiave[] = { string("id") };
StringField DBEObject::chiave1( DBEObject::nomiCampiChiave[0] );
DBFieldVector DBEObject::chiavi = DBEObject::___init_keys();
DBFieldVector DBEObject::___init_keys() { DBFieldVector ret = DBFieldVector(); ret.push_back( &DBEObject::chiave1 ); return ret; }
DBEObject::DBEObject() { this->tableName.clear(); }
DBEObject::~DBEObject() {}
string DBEObject::name() const { return "DBEObject"; }
string DBEObject::getTableName() const { return "objects"; }
DBFieldVector* DBEObject::getKeys() const { return &DBEObject::chiavi; }
DBEObject* DBEObject::createNewInstance() const { return new DBEObject(); }
//*********************** DBEObject: end.

//*********************** DBECountry: start.
const string DBECountry::nomiCampiChiave[] = { string("id") };
StringField DBECountry::chiave1( DBECountry::nomiCampiChiave[0] );
DBFieldVector DBECountry::chiavi = DBECountry::___init_keys();
DBFieldVector DBECountry::___init_keys() { DBFieldVector ret = DBFieldVector(); ret.push_back( &DBECountry::chiave1 ); return ret; }
DBECountry::DBECountry() { this->tableName.clear(); }
DBECountry::~DBECountry() {}
string DBECountry::name() const { return "DBECountry"; }
string DBECountry::getTableName() const { return "countrylist"; }
DBFieldVector* DBECountry::getKeys() const { return &DBECountry::chiavi; }
DBECountry* DBECountry::createNewInstance() const { return new DBECountry(); }
//*********************** DBECountry: end.

//*********************** DBECompany: start.
const string DBECompany::nomiCampiChiave[] = { string("id") };
StringField DBECompany::chiave1( DBECompany::nomiCampiChiave[0] );
DBFieldVector DBECompany::chiavi = DBECompany::___init_keys();
DBFieldVector DBECompany::___init_keys() { DBFieldVector ret = DBFieldVector(); ret.push_back( &DBECompany::chiave1 ); return ret; }
DBECompany::DBECompany() { this->tableName.clear(); }
DBECompany::~DBECompany() {}
string DBECompany::name() const { return "DBECompany"; }
string DBECompany::getTableName() const { return "companies"; }
DBFieldVector* DBECompany::getKeys() const { return &DBECompany::chiavi; }
DBECompany* DBECompany::createNewInstance() const { return new DBECompany(); }
//*********************** DBECompany: end.

//*********************** DBEPeople: start.
const string DBEPeople::nomiCampiChiave[] = { string("id") };
StringField DBEPeople::chiave1( DBEPeople::nomiCampiChiave[0] );
DBFieldVector DBEPeople::chiavi = DBEPeople::___init_keys();
DBFieldVector DBEPeople::___init_keys() { DBFieldVector ret = DBFieldVector(); ret.push_back( &DBEPeople::chiave1 ); return ret; }
DBEPeople::DBEPeople() { this->tableName.clear(); }
DBEPeople::~DBEPeople() {}
string DBEPeople::name() const { return "DBEPeople"; }
string DBEPeople::getTableName() const { return "people"; }
DBFieldVector* DBEPeople::getKeys() const { return &DBEPeople::chiavi; }
DBEPeople* DBEPeople::createNewInstance() const { return new DBEPeople(); }
//*********************** DBEPeople: end.




//*********************** DBEFile: start.
const string DBEFile::nomiCampiChiave[] = { string("id") };
StringField DBEFile::chiave1( DBEFile::nomiCampiChiave[0] );
DBFieldVector DBEFile::chiavi = DBEFile::___init_keys();
DBFieldVector DBEFile::___init_keys() { DBFieldVector ret = DBFieldVector(); ret.push_back( &DBEFile::chiave1 ); return ret; }
DBEFile::DBEFile() { this->tableName.clear(); }
DBEFile::~DBEFile() {}
string DBEFile::name() const { return "DBEFile"; }
string DBEFile::getTableName() const { return "files"; }
DBFieldVector* DBEFile::getKeys() const { return &DBEFile::chiavi; }
DBEFile* DBEFile::createNewInstance() const { return new DBEFile(); }
//*********************** DBEFile: end.

//*********************** DBEFolder: start.
const string DBEFolder::nomiCampiChiave[] = { string("id") };
StringField DBEFolder::chiave1( DBEFolder::nomiCampiChiave[0] );
DBFieldVector DBEFolder::chiavi = DBEFolder::___init_keys();
DBFieldVector DBEFolder::___init_keys() { DBFieldVector ret = DBFieldVector(); ret.push_back( &DBEFolder::chiave1 ); return ret; }
DBEFolder::DBEFolder() { this->tableName.clear(); }
DBEFolder::~DBEFolder() {}
string DBEFolder::name() const { return "DBEFolder"; }
string DBEFolder::getTableName() const { return "folders"; }
DBFieldVector* DBEFolder::getKeys() const { return &DBEFolder::chiavi; }
DBEFolder* DBEFolder::createNewInstance() const { return new DBEFolder(); }
//*********************** DBEFolder: end.

//*********************** DBELink: start.
const string DBELink::nomiCampiChiave[] = { string("id") };
StringField DBELink::chiave1( DBELink::nomiCampiChiave[0] );
DBFieldVector DBELink::chiavi = DBELink::___init_keys();
DBFieldVector DBELink::___init_keys() { DBFieldVector ret = DBFieldVector(); ret.push_back( &DBELink::chiave1 ); return ret; }
DBELink::DBELink() { this->tableName.clear(); }
DBELink::~DBELink() {}
string DBELink::name() const { return "DBELink"; }
string DBELink::getTableName() const { return "links"; }
DBFieldVector* DBELink::getKeys() const { return &DBELink::chiavi; }
DBELink* DBELink::createNewInstance() const { return new DBELink(); }
//*********************** DBELink: end.

//*********************** DBENote: start.
const string DBENote::nomiCampiChiave[] = { string("id") };
StringField DBENote::chiave1( DBENote::nomiCampiChiave[0] );
DBFieldVector DBENote::chiavi = DBENote::___init_keys();
DBFieldVector DBENote::___init_keys() { DBFieldVector ret = DBFieldVector(); ret.push_back( &DBENote::chiave1 ); return ret; }
DBENote::DBENote() { this->tableName.clear(); }
DBENote::~DBENote() {}
string DBENote::name() const { return "DBENote"; }
string DBENote::getTableName() const { return "notes"; }
DBFieldVector* DBENote::getKeys() const { return &DBENote::chiavi; }
DBENote* DBENote::createNewInstance() const { return new DBENote(); }
//*********************** DBENote: end.

//*********************** DBEPage: start.
const string DBEPage::nomiCampiChiave[] = { string("id") };
StringField DBEPage::chiave1( DBEPage::nomiCampiChiave[0] );
DBFieldVector DBEPage::chiavi = DBEPage::___init_keys();
DBFieldVector DBEPage::___init_keys() { DBFieldVector ret = DBFieldVector(); ret.push_back( &DBEPage::chiave1 ); return ret; }
DBEPage::DBEPage() { this->tableName.clear(); }
DBEPage::~DBEPage() {}
string DBEPage::name() const { return "DBEPage"; }
string DBEPage::getTableName() const { return "pages"; }
DBFieldVector* DBEPage::getKeys() const { return &DBEPage::chiavi; }
DBEPage* DBEPage::createNewInstance() const { return new DBEPage(); }
//*********************** DBEPage: end.

string CMSchema::getSchema() { return "cm"; }
void CMSchema::registerClasses(DBEFactory* dbeFactory) {
    dbeFactory->registerClass("log", new DBELog() );
    dbeFactory->registerClass("objects", new DBEObject() );
    dbeFactory->registerClass("countrylist", new DBECountry() );
    dbeFactory->registerClass("companies", new DBECompany() );
    dbeFactory->registerClass("people", new DBEPeople() );
    dbeFactory->registerClass("files", new DBEFile() );
    dbeFactory->registerClass("folders", new DBEFolder() );
    dbeFactory->registerClass("links", new DBELink() );
    dbeFactory->registerClass("notes", new DBENote() );
    dbeFactory->registerClass("pages", new DBEPage() );
}
