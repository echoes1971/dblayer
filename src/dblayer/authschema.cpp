/**
 * dbschema.cpp
 */

#include "authschema.h"
using namespace AuthSchema;

#include <string>
using namespace std;

//*********************** DBEDBVersion: start.
const string DBEDBVersion::nomiCampiChiave[] = { string("model_name") };
ColumnDefinitions DBEDBVersion::_columns;
ColumnDefinitions DBEDBVersion::getColumns() const { return DBEDBVersion::_columns; }
IntegerField DBEDBVersion::chiave1( (const string*)&DBEDBVersion::nomiCampiChiave[0] );
DBFieldVector DBEDBVersion::chiavi = DBEDBVersion::___init_keys();
DBFieldVector DBEDBVersion::___init_keys() { DBFieldVector ret = DBFieldVector(); ret.push_back( &DBEDBVersion::chiave1 ); return ret; }
DBEDBVersion::DBEDBVersion() {
    this->tableName.clear();
    if(DBEDBVersion::_columns.size()==0) {
        for(const pair<string,vector<string> > pair: DBEntity::getColumns()) {
            DBEDBVersion::_columns[pair.first] = pair.second;
        }
        DBEDBVersion::_columns["model_name"] = vector<string> {"varchar(255)","not null"};
        DBEDBVersion::_columns["version"] = vector<string> {"int","not null"};
    }
}
DBEDBVersion::~DBEDBVersion() {}
string DBEDBVersion::name() const { return "DBEDBVersion"; }
string DBEDBVersion::getTableName() const { return "dbversion"; }
DBFieldVector* DBEDBVersion::getKeys() const { return &DBEDBVersion::chiavi; }
DBEDBVersion* DBEDBVersion::createNewInstance() const { return new DBEDBVersion(); }

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
ColumnDefinitions DBEUser::_columns;
ForeignKeyVector DBEUser::_fkv;
ColumnDefinitions DBEUser::getColumns() const { return DBEUser::_columns; }
StringField DBEUser::chiave1( (const string*)&DBEUser::nomiCampiChiave[0] );
DBFieldVector DBEUser::chiavi = DBEUser::___init_keys();
DBFieldVector DBEUser::___init_keys() { DBFieldVector ret = DBFieldVector(); ret.push_back( &DBEUser::chiave1 ); return ret; }
DBEUser::DBEUser() {
    this->tableName.clear();
    if(DBEUser::_columns.size()==0) {
        for(const pair<string,vector<string> > pair: DBEntity::getColumns()) {
            DBEUser::_columns[pair.first] = pair.second;
        }
        DBEUser::_columns["id"] = vector<string> {"uuid","not null"};
        DBEUser::_columns["login"] = vector<string> {"varchar(255)","not null"};
        DBEUser::_columns["pwd"] = vector<string> {"varchar(255)","not null"};
        DBEUser::_columns["pwd_salt"] = vector<string> {"varchar(4)","default ''"};
        DBEUser::_columns["fullname"] = vector<string> {"text","default null"};
        DBEUser::_columns["group_id"] = vector<string> {"uuid","not null"};
    }
}
DBEUser::~DBEUser() {}
string DBEUser::name() const { return "DBEUser"; }
string DBEUser::getTableName() const { return "users"; }
DBFieldVector* DBEUser::getKeys() const { return &DBEUser::chiavi; }
ForeignKeyVector& DBEUser::getFK() const {
    if(_fkv.size()==0) {
        for(const auto& fk : DBEntity::getFK()) {
            _fkv.push_back(fk);
        }
        _fkv.push_back(ForeignKey("group_id","groups","id"));
    }
    return _fkv;
}
DBEUser* DBEUser::createNewInstance() const { return new DBEUser(); }
DBLayer::StringVector DBEUser::getOrderBy() const {
    static DBLayer::StringVector ret({"fullname"});
    return ret;
}
vector<map<string,string> > DBEUser::getDefaultEntries() const {
    static vector<map<string,string> > ret;
    if(ret.size()==0) {
        int i=0;
        ret.push_back( map<string,string>() );
        ret.at(i)["id"]="-1"; ret.at(i)["login"]="adm"; ret.at(i)["pwd"]="adm";
        ret.at(i)["pwd_salt"]=""; ret.at(i)["fullname"]="Administrator"; ret.at(i)["group_id"]="-2";
        i++;
    }
    return ret;
}
bool DBEUser::isRoot() {
    static string id("id");
    return string(this->getField(&id)->getStringValue()->c_str())=="-1";
}

void DBEUser::_before_insert(DBMgr* dbmgr) {
    static string id("id");
    if(this->isNull(&id)) {
        this->getField(&id)->setStringValue(dbmgr->getNextUuid(this));
    }
    if(this->checkNewPassword()) {
        this->_createGroup(dbmgr);
    }
}
void DBEUser::_after_insert(DBMgr* dbmgr) {
    this->_checkGroupAssociation(dbmgr);
}
void DBEUser::_after_update(DBMgr* dbmgr) {
    this->_checkGroupAssociation(dbmgr);
}
void DBEUser::_after_delete(DBMgr* dbmgr) {
    static string user_id("user_id");
    static string id("id");
    DBEUserGroup search;
    search.getField(&user_id)->setStringValue(string(this->getStringValue(&id).c_str()));
    DBEntityVector* list = dbmgr->Search(&search,false);
    for(DBEntity* dbe : (*list)) {
        dbmgr->Delete(dbe);
    }
    dbmgr->Destroy(list);
    this->_deleteGroup(dbmgr);
}
void DBEUser::_createGroup(DBMgr* dbmgr) {
    static string name("name");
    static string description("description");
    static string login("login");
    static string group_id("group_id");
    static string id("id");
    if(!this->getField(&group_id)->isNull())
        return;
    DBEGroup* dbe = new DBEGroup();
    dbe->getField(&name)->setStringValue(string(this->getStringValue(&login).c_str()));
    dbe->getField(&description)->setStringValue("Private group for "+string(this->getStringValue(&id).c_str())+"-"+string(this->getStringValue(&login).c_str()));
    dbe = (DBEGroup*) dbmgr->Insert(dbe);
    this->getField(&group_id)->setStringValue(string(dbe->getStringValue(&id).c_str()));
    delete dbe;
}
void DBEUser::_deleteGroup(DBMgr* dbmgr) {
    static string group_id("group_id");
    static string id("id");
    DBEGroup dbe;
    dbe.getField(&id)->setStringValue(string(this->getField(&group_id)->getStringValue()->c_str()));
    dbmgr->Delete(&dbe);
}
void DBEUser::_checkGroupAssociation(DBMgr* dbmgr) {
    static string user_id("user_id");
    static string group_id("group_id");
    static string id("id");
    DBEUserGroup ug;
    ug.getField(&user_id)->setStringValue(this->getField(&id)->getStringValue());
    ug.getField(&group_id)->setStringValue(this->getField(&group_id)->getStringValue());
    bool exists = dbmgr->exists(&ug);
    if(!exists) {
        dbmgr->Insert(&ug);
    }
}
//*********************** DBEUser: end.

//*********************** DBEGroup: start.
const string DBEGroup::nomiCampiChiave[] = { string("id") };
ColumnDefinitions DBEGroup::_columns;
ColumnDefinitions DBEGroup::getColumns() const { return DBEGroup::_columns; }
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
string DBEGroup::name() const { return "DBEGroup"; }
string DBEGroup::getTableName() const { return "groups"; }
DBFieldVector* DBEGroup::getKeys() const { return &DBEGroup::chiavi; }
DBEGroup* DBEGroup::createNewInstance() const { return new DBEGroup(); }

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

void DBEGroup::_before_insert(DBMgr* dbmgr) {
    static const string id_field_name;
    Field* id_field = this->getField(&id_field_name);
    if(id_field->isNull()) {
        string uuid = dbmgr->getNextUuid(this);
        id_field->setStringValue(uuid);
    }
}
void DBEGroup::_after_insert(DBMgr* dbmgr) {
    if(dbmgr->getDBEUser()==0)
        return;
    DBEUserGroup dbe;
    static string group_id("group_id");
    static string user_id("user_id");
    static string id("id");

    string this_id = string(this->getField(&id)->getStringValue()->c_str());

    dbe.getField(&group_id);
    dbe.getField(&user_id),dbmgr->getDBEUser()->getField(&id)->getStringValue();
    dbmgr->Insert(&dbe);
    dbmgr->addGroup(this_id);
}
void DBEGroup::_after_delete(DBMgr* dbmgr) {
    static const string group_id("group_id");
    static const string id("id");
    DBEUserGroup search;
    search.getField(&group_id)->setStringValue(this->getField(&id)->getStringValue());
    DBEntityVector* list = dbmgr->Search(&search,false);
    for(DBEntity* dbe : (*list)) {
        dbmgr->Delete(dbe);
    }
    dbmgr->Destroy(list);
}

//*********************** DBEGroup: end.

//*********************** DBEUserGroup: start.
const string DBEUserGroup::nomiCampiChiave[] = { string("user_id"), string("group_id") };
ColumnDefinitions DBEUserGroup::_columns;
ForeignKeyVector DBEUserGroup::_fkv;
ColumnDefinitions DBEUserGroup::getColumns() const { return DBEUserGroup::_columns; }
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
string DBEUserGroup::name() const { return "DBEUserGroup"; }
string DBEUserGroup::getTableName() const { return "users_groups"; }
DBFieldVector* DBEUserGroup::getKeys() const { return &DBEUserGroup::chiavi; }
ForeignKeyVector& DBEUserGroup::getFK() const {
    if(_fkv.size()==0) {
        for(const auto& fk : DBEntity::getFK()) {
            _fkv.push_back(fk);
        }
        _fkv.push_back(ForeignKey("user_id","users","id"));
        _fkv.push_back(ForeignKey("group_id","groups","id"));
    }
    return _fkv;
}

DBEUserGroup* DBEUserGroup::createNewInstance() const { return new DBEUserGroup(); }

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
ColumnDefinitions DBELog::_columns;
ColumnDefinitions DBELog::getColumns() const { return DBELog::_columns; }
StringField DBELog::chiave1( (const string*)&DBELog::nomiCampiChiave[0] );
StringField DBELog::chiave2( (const string*)&DBELog::nomiCampiChiave[1] );
DBFieldVector DBELog::chiavi = DBELog::___init_keys();
DBFieldVector DBELog::___init_keys() { DBFieldVector ret = DBFieldVector(); ret.push_back( &DBELog::chiave1 ); ret.push_back( &DBELog::chiave2 ); return ret; }
DBELog::DBELog() {
    this->tableName.clear();
    if(DBELog::_columns.size()==0) {
        for(const pair<string,vector<string> > pair: DBEntity::getColumns()) {
            DBELog::_columns[pair.first] = pair.second;
        }
        DBELog::_columns["ip"] = vector<string> {"varchar(16)","not null"};
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


string AuthSchema::getSchema() { return "auth"; }
void AuthSchema::registerClasses(DBEFactory* dbeFactory) {
  dbeFactory->registerClass("dbversion", new DBEDBVersion() );
  dbeFactory->registerClass("users", new DBEUser() );
  dbeFactory->registerClass("groups", new DBEGroup() );
  dbeFactory->registerClass("users_groups", new DBEUserGroup() );
  dbeFactory->registerClass("log", new DBELog() );
}

void AuthSchema::checkDB(DBMgr& dbmgr) {
    cout << "AuthSchema::checkDB: TODO" << endl;

    DBEDBVersion dbversion;
    dbversion.setSchemaName(dbmgr.getSchema());
    cout << "AuthSchema::checkDB: " << dbversion.toSql("\n") << endl;
}
