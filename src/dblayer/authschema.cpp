/**
 * dbschema.cpp
 */

#include "authschema.h"
using namespace AuthSchema;

#include <string>
using namespace std;

//*********************** DBEDBVersion: start.
const string DBEDBVersion::nomiCampiChiave[] = { string("version") };
ColumnDefinitions DBEDBVersion::_columns;
ColumnDefinitions DBEDBVersion::getColumns() { return DBEDBVersion::_columns; }
IntegerField DBEDBVersion::chiave1( (const string*)&DBEDBVersion::nomiCampiChiave[0] );
DBFieldVector DBEDBVersion::chiavi = DBEDBVersion::___init_keys();
DBFieldVector DBEDBVersion::___init_keys() { DBFieldVector ret = DBFieldVector(); ret.push_back( &DBEDBVersion::chiave1 ); return ret; }
DBEDBVersion::DBEDBVersion() {
    this->tableName.clear();
    if(DBEDBVersion::_columns.size()==0) {
        for(const pair<string,vector<string> > pair: DBEntity::getColumns()) {
            DBEDBVersion::_columns[pair.first] = pair.second;
        }
        DBEDBVersion::_columns["version"] = vector<string> {"int","not null"};
    }
}
DBEDBVersion::~DBEDBVersion() {}
string DBEDBVersion::name() { return "DBEDBVersion"; }
string DBEDBVersion::getTableName() { return "dbversion"; }
DBFieldVector* DBEDBVersion::getKeys() { return &DBEDBVersion::chiavi; }
DBEDBVersion* DBEDBVersion::createNewInstance() { return new DBEDBVersion(); }

DBLayer::StringVector DBEDBVersion::getOrderBy() const {
    static DBLayer::StringVector ret({"version"});
    return ret;
}

int DBEDBVersion::version() {
    string field_name("version");
    return this->getField(&field_name)->getIntegerValue();
}

//*********************** DBEDBVersion: end.

//*********************** DBEUser: start.
const string DBEUser::nomiCampiChiave[] = { string("id") };
StringField DBEUser::chiave1( (const string*)&DBEUser::nomiCampiChiave[0] );
DBFieldVector DBEUser::chiavi = DBEUser::___init_keys();
DBFieldVector DBEUser::___init_keys() { DBFieldVector ret = DBFieldVector(); ret.push_back( &DBEUser::chiave1 ); return ret; }
DBEUser::DBEUser() { this->tableName.clear(); }
DBEUser::~DBEUser() {}
string DBEUser::name() { return "DBEUser"; }
string DBEUser::getTableName() { return "users"; }
DBFieldVector* DBEUser::getKeys() { return &DBEUser::chiavi; }
DBEUser* DBEUser::createNewInstance() { return new DBEUser(); }
//*********************** DBEUser: end.

//*********************** DBEGroup: start.
const string DBEGroup::nomiCampiChiave[] = { string("id") };
ColumnDefinitions DBEGroup::_columns;
ColumnDefinitions DBEGroup::getColumns() { return DBEGroup::_columns; }
StringField DBEGroup::chiave1( (const string*)&DBEGroup::nomiCampiChiave[0] );
DBFieldVector DBEGroup::chiavi = DBEGroup::___init_keys();
DBFieldVector DBEGroup::___init_keys() { DBFieldVector ret = DBFieldVector(); ret.push_back( &DBEGroup::chiave1 ); return ret; }
DBEGroup::DBEGroup() {
    this->tableName.clear();
    if(DBEGroup::_columns.size()==0) {
        for(const pair<string,vector<string> > pair: DBEntity::getColumns()) {
            DBEGroup::_columns[pair.first] = pair.second;
        }
        DBEGroup::_columns["id"] = vector<string> {"uuid","not null"};
        DBEGroup::_columns["name"] = vector<string> {"varchar(255)","not null"};
        DBEGroup::_columns["description"] = vector<string> {"text","default null"};
    }
}
DBEGroup::~DBEGroup() {}
string DBEGroup::name() { return "DBEGroup"; }
string DBEGroup::getTableName() { return "groups"; }
DBFieldVector* DBEGroup::getKeys() { return &DBEGroup::chiavi; }
DBEGroup* DBEGroup::createNewInstance() { return new DBEGroup(); }

DBLayer::StringVector DBEGroup::getOrderBy() const {
    static DBLayer::StringVector ret({"name"});
    return ret;
}

vector<map<string,string> > DBEGroup::getDefaultEntries() const {
    static vector<map<string,string> > ret;
    if(ret.size()==0) {
        int i=0;
        ret.push_back( map<string,string>() );
        ret.at(i)["id"]="-2"; ret.at(i)["name"]="Admin"; ret.at(i)["description"]="System admins"; i++;
        ret.push_back( map<string,string>() );
        ret.at(i)["id"]="-3"; ret.at(i)["name"]="Users"; ret.at(i)["description"]="System users"; i++;
        ret.push_back( map<string,string>() );
        ret.at(i)["id"]="-4"; ret.at(i)["name"]="Guests"; ret.at(i)["description"]="System guests (read only)"; i++;
//         ret.push_back( map<string,string>() );
//         ret.at(i)["id"]="-5"; ret.at(i)["name"]="Project"; ret.at(i)["description"]="R-Project user"; i++;
//         ret.push_back( map<string,string>() );
//         ret.at(i)["id"]="-6"; ret.at(i)["name"]="Webmaster"; ret.at(i)["description"]="Web content creators"; i++;
    }
    return ret;
}

//*********************** DBEGroup: end.

//*********************** DBEUserGroup: start.
const string DBEUserGroup::nomiCampiChiave[] = { string("user_id"), string("group_id") };
ColumnDefinitions DBEUserGroup::_columns;
ForeignKeyVector DBEUserGroup::_fkv;
ColumnDefinitions DBEUserGroup::getColumns() { return DBEUserGroup::_columns; }
StringField DBEUserGroup::chiave1( (const string*)&DBEUserGroup::nomiCampiChiave[0] );
StringField DBEUserGroup::chiave2( (const string*)&DBEUserGroup::nomiCampiChiave[1] );
DBFieldVector DBEUserGroup::chiavi = DBEUserGroup::___init_keys();
DBFieldVector DBEUserGroup::___init_keys() { DBFieldVector ret = DBFieldVector(); ret.push_back( &DBEUserGroup::chiave1 ); ret.push_back( &DBEUserGroup::chiave2 ); return ret; }
DBEUserGroup::DBEUserGroup() {
    this->tableName.clear();
    if(DBEUserGroup::_columns.size()==0) {
        for(const pair<string,vector<string> > pair: DBEntity::getColumns()) {
            DBEUserGroup::_columns[pair.first] = pair.second;
        }
        DBEUserGroup::_columns["user_id"] = vector<string> {"uuid","not null"};
        DBEUserGroup::_columns["group_id"] = vector<string> {"uuid","not null"};
    }
}
DBEUserGroup::~DBEUserGroup() {}
string DBEUserGroup::name() { return "DBEUserGroup"; }
string DBEUserGroup::getTableName() { return "users_groups"; }
DBFieldVector* DBEUserGroup::getKeys() { return &DBEUserGroup::chiavi; }
ForeignKeyVector& DBEUserGroup::getFK() {
    if(_fkv.size()==0) {
        for(const auto& fk : DBEntity::getFK()) {
            _fkv.push_back(fk);
        }
        _fkv.push_back(ForeignKey("user_id","users","id"));
        _fkv.push_back(ForeignKey("group_id","groups","id"));
    }
    return _fkv;
}

DBEUserGroup* DBEUserGroup::createNewInstance() { return new DBEUserGroup(); }

DBLayer::StringVector DBEUserGroup::getOrderBy() const {
    static DBLayer::StringVector ret({"user_id","group_id"});
    return ret;
}


vector<map<string,string> > DBEUserGroup::getDefaultEntries() const {
    static vector<map<string,string> > ret;
    if(ret.size()==0) {
        int i=0;
        ret.push_back( map<string,string>() ); ret.at(i)["user_id"]="-1"; ret.at(i)["group_id"]="-2"; i++;
        ret.push_back( map<string,string>() ); ret.at(i)["user_id"]="-1"; ret.at(i)["group_id"]="-5"; i++;
        ret.push_back( map<string,string>() ); ret.at(i)["user_id"]="-1"; ret.at(i)["group_id"]="-6"; i++;
    }
    return ret;
}
//*********************** DBEUserGroup: end.

//*********************** DBELog: start.
const string DBELog::nomiCampiChiave[] = { string("ip"), string("data") };
StringField DBELog::chiave1( (const string*)&DBELog::nomiCampiChiave[0] );
StringField DBELog::chiave2( (const string*)&DBELog::nomiCampiChiave[1] );
DBFieldVector DBELog::chiavi = DBELog::___init_keys();
DBFieldVector DBELog::___init_keys() { DBFieldVector ret = DBFieldVector(); ret.push_back( &DBELog::chiave1 ); ret.push_back( &DBELog::chiave2 ); return ret; }
DBELog::DBELog() { this->tableName.clear(); }
DBELog::~DBELog() {}
string DBELog::name() { return "DBELog"; }
string DBELog::getTableName() { return "log"; }
DBFieldVector* DBELog::getKeys() { return &DBELog::chiavi; }
DBELog* DBELog::createNewInstance() { return new DBELog(); }
//*********************** DBELog: end.


string AuthSchema::getSchema() { return "auth"; }
void AuthSchema::registerClasses(DBEFactory* dbeFactory) {
  dbeFactory->registerClass("dbversion", new DBEDBVersion() );
  dbeFactory->registerClass("users", new DBEUser() );
  dbeFactory->registerClass("groups", new DBEGroup() );
  dbeFactory->registerClass("users_groups", new DBEUserGroup() );
  dbeFactory->registerClass("log", new DBELog() );
}
