/**
 * dbschema.cpp
 */

#include "authschema.h"
using namespace AuthSchema;

#include <string>
using namespace std;

//*********************** DBEDBVersion: start.
StringVector DBEDBVersion::chiavi = { "model_name" };
ColumnDefinitions DBEDBVersion::_columns;
StringVector DBEDBVersion::_column_order = {"model_name","version"};
ColumnDefinitions DBEDBVersion::getColumns() const { return _columns; }
StringVector DBEDBVersion::getColumnNames() const { return _column_order; }
DBEDBVersion::DBEDBVersion() {
    this->tableName.clear();
    this->schemaName = "dblayer";
    if(_columns.size()==0) {
        for(const pair<string,vector<string> > pair: DBEntity::getColumns()) {
            _columns[pair.first] = pair.second;
        }
        _columns["model_name"] = vector<string> {"varchar(255)","not null"};
        _columns["version"] = vector<string> {"int","not null"};
    }
}
DBEDBVersion::~DBEDBVersion() {}
string DBEDBVersion::name() const { return "DBEDBVersion"; }
string DBEDBVersion::getTableName() const { return "dbversion"; }
StringVector DBEDBVersion::getKeys() const { return DBEDBVersion::chiavi; }
DBEDBVersion* DBEDBVersion::createNewInstance() const { return new DBEDBVersion(); }
DBLayer::StringVector DBEDBVersion::getOrderBy() const {
    static DBLayer::StringVector ret({"version"});
    return ret;
}

int DBEDBVersion::version() {
    return this->getField("version")->getIntegerValue();
}
//*********************** DBEDBVersion: end.

//*********************** DBEUser: start.
StringVector DBEUser::chiavi = { "id" };
ForeignKeyVector DBEUser::_fkv;
ColumnDefinitions DBEUser::_columns;
StringVector DBEUser::_column_order = {"id","login","pwd","pwd_salt","fullname","group_id"};
DBEUser::DBEUser() {
    this->tableName.clear();
    this->schemaName = AuthSchema::getSchema();
    if(DBEUser::_columns.size()==0) {
        for(const pair<string,vector<string> > pair: DBEntity::getColumns()) {
            _columns[pair.first] = pair.second;
        }
        _columns["id"] = vector<string> {"uuid","not null"};
        _columns["login"] = vector<string> {"varchar(255)","not null"};
        _columns["pwd"] = vector<string> {"varchar(255)","not null"};
        _columns["pwd_salt"] = vector<string> {"varchar(4)","default ''"};
        _columns["fullname"] = vector<string> {"text","default null"};
        _columns["group_id"] = vector<string> {"uuid","not null"};
    }
    if(_fkv.size()==0) {
        for(const auto& fk : DBEntity::getFK()) { _fkv.push_back(fk); }
        _fkv.push_back(ForeignKey("group_id","groups","id"));
    }
}
DBEUser::~DBEUser() {}
string DBEUser::name() const { return "DBEUser"; }
string DBEUser::getTableName() const { return "users"; }
StringVector DBEUser::getKeys() const { return DBEUser::chiavi; }
ForeignKeyVector& DBEUser::getFK() const { return _fkv; }
ColumnDefinitions DBEUser::getColumns() const { return _columns; }
StringVector DBEUser::getColumnNames() const { return _column_order; }
DBEUser* DBEUser::createNewInstance() const { return new DBEUser(); }
DBLayer::StringVector DBEUser::getOrderBy() const {
    static DBLayer::StringVector ret({"fullname"});
    return ret;
}
string DBEUser::getId() const { return this->getStringValue("id"); }
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
    return string(this->getField("id")->getStringValue()->c_str())=="-1";
}

void DBEUser::_before_insert(DBMgr* dbmgr) {
    if(this->isNull("id")) {
        this->setValue("id",dbmgr->getNextUuid(this));
//        this->getField("id")->setStringValue(dbmgr->getNextUuid(this));
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
    DBEUserGroup search;
    search.setValue("user_id",this->getStringValue("id"));
//    search.getField("user_id")->setStringValue(string(this->getStringValue("id").c_str()));
    DBEntityVector* list = dbmgr->Search(&search,false);
    for(DBEntity* dbe : (*list)) {
        dbmgr->Delete(dbe);
    }
    dbmgr->Destroy(list);
    this->_deleteGroup(dbmgr);
}
void DBEUser::_createGroup(DBMgr* dbmgr) {
    if(!this->getField("group_id")->isNull())
        return;
    DBEGroup* dbe = new DBEGroup();
    dbe->setValue("name",this->getStringValue("login"));
    dbe->setValue("description","Private group for "+string(this->getStringValue("id").c_str())+"-"+string(this->getStringValue("login").c_str()));
//    dbe->getField("name")->setStringValue(string(this->getStringValue("login").c_str()));
//    dbe->getField("description")->setStringValue("Private group for "+string(this->getStringValue("id").c_str())+"-"+string(this->getStringValue("login").c_str()));
    dbe = (DBEGroup*) dbmgr->Insert(dbe);
    this->setValue("group_id",dbe->getStringValue("id"));
//    this->getField("group_id")->setStringValue(string(dbe->getStringValue("id").c_str()));
    delete dbe;
}
void DBEUser::_deleteGroup(DBMgr* dbmgr) {
    DBEGroup dbe;
    dbe.setValue("id",this->getStringValue("group_id"));
    //dbe.getField("id")->setStringValue(string(this->getField("group_id")->getStringValue()->c_str()));
    dbmgr->Delete(&dbe);
}
void DBEUser::_checkGroupAssociation(DBMgr* dbmgr) {
    DBEUserGroup ug;
    ug.setValue("user_id",this->getStringValue("id"));
    ug.setValue("group_id",this->getStringValue("group_id"));
    //ug.getField("user_id")->setStringValue(this->getField("id")->getStringValue());
    //ug.getField("group_id")->setStringValue(this->getField("group_id")->getStringValue());
    bool exists = dbmgr->exists(&ug);
    if(!exists) {
        dbmgr->Insert(&ug);
    }
}
//*********************** DBEUser: end.

//*********************** DBEGroup: start.
StringVector DBEGroup::chiavi = { "id" };
ColumnDefinitions DBEGroup::_columns;
StringVector DBEGroup::_column_order = {"id","name","description"};
DBEGroup::DBEGroup() {
    this->tableName.clear();
    this->schemaName = AuthSchema::getSchema();
    if(_columns.size()==0) {
        for(const pair<string,vector<string> > pair: DBEntity::getColumns()) {
            _columns[pair.first] = pair.second;
        }
        _columns["id"] = vector<string> {"uuid","not null"};
        _columns["name"] = vector<string> {"varchar(255)","not null"};
        _columns["description"] = vector<string> {"text","default null"};
    }
}
DBEGroup::~DBEGroup() {}
string DBEGroup::name() const { return "DBEGroup"; }
string DBEGroup::getTableName() const { return "groups"; }
DBEGroup* DBEGroup::createNewInstance() const { return new DBEGroup(); }
StringVector DBEGroup::getKeys() const { return chiavi; }
ColumnDefinitions DBEGroup::getColumns() const { return _columns; }
StringVector DBEGroup::getColumnNames() const { return _column_order; }
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
    Field* id_field = this->getField("id");
    if(id_field->isNull()) {
        string uuid = dbmgr->getNextUuid(this);
        id_field->setStringValue(uuid);
    }
}
void DBEGroup::_after_insert(DBMgr* dbmgr) {
    if(dbmgr->getDBEUser()==0)
        return;
    DBEUserGroup dbe;

    string this_id = string(this->getField("id")->getStringValue()->c_str());

    dbe.setValue("group_id",this_id);
    dbe.setValue("user_id",dbmgr->getDBEUser()->getField("id")->getStringValue());
    dbmgr->Insert(&dbe);
    dbmgr->addGroup(this_id);
}
void DBEGroup::_after_delete(DBMgr* dbmgr) {
    DBEUserGroup search;
    search.getField("group_id")->setStringValue(this->getField("id")->getStringValue());
    DBEntityVector* list = dbmgr->Search(&search,false);
    for(DBEntity* dbe : (*list)) {
        dbmgr->Delete(dbe);
    }
    dbmgr->Destroy(list);
}

//*********************** DBEGroup: end.

//*********************** DBEUserGroup: start.
StringVector DBEUserGroup::chiavi = {"user_id","group_id"};
ForeignKeyVector DBEUserGroup::_fkv;
ColumnDefinitions DBEUserGroup::_columns;
StringVector DBEUserGroup::_column_order = {"user_id","group_id"};
DBEUserGroup::DBEUserGroup() {
    this->tableName.clear();
    this->schemaName = AuthSchema::getSchema();
    if(_columns.size()==0) {
        for(const pair<string,vector<string> > pair: DBEntity::getColumns()) {
            _columns[pair.first] = pair.second;
        }
        _columns["user_id"] = vector<string> {"uuid","not null"};
        _columns["group_id"] = vector<string> {"uuid","not null"};
    }
    if(_fkv.size()==0) {
        for(const auto& fk : DBEntity::getFK()) {
            _fkv.push_back(fk);
        }
        _fkv.push_back(ForeignKey("user_id","users","id"));
        _fkv.push_back(ForeignKey("group_id","groups","id"));
    }
}
DBEUserGroup::~DBEUserGroup() {}
string DBEUserGroup::name() const { return "DBEUserGroup"; }
string DBEUserGroup::getTableName() const { return "users_groups"; }
StringVector DBEUserGroup::getKeys() const { return chiavi; }
ForeignKeyVector& DBEUserGroup::getFK() const { return _fkv; }
ColumnDefinitions DBEUserGroup::getColumns() const { return _columns; }
StringVector DBEUserGroup::getColumnNames() const { return _column_order; }
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


string AuthSchema::getSchema() { return "auth"; }
void AuthSchema::registerClasses(DBEFactory* dbeFactory) {
  dbeFactory->registerClass("dbversion", new DBEDBVersion() );
  dbeFactory->registerClass("users", new DBEUser() );
  dbeFactory->registerClass("groups", new DBEGroup() );
  dbeFactory->registerClass("users_groups", new DBEUserGroup() );
}

void AuthSchema::checkDB(DBMgr& dbmgr, bool verbose) {
    if(verbose) cout << "AuthSchema::checkDB: start." << endl;

    if(dbmgr.getConnection()->isProxy()) {
        cerr << "AuthSchema::checkDB: WARNING - remote databases cannot be checked." << endl;
        return;
    }
    if(!dbmgr.connect()) {
        cerr << "AuthSchema::checkDB: ERROR - UNABLE TO CONNECT TO DB!!!" << endl;
        return;
    }

    // 1. Check app version
    long current_db_version = -1;
    DBEDBVersion* dbecurrentversion;
    DBEDBVersion* cerca = new DBEDBVersion();
    cerca->setValue("model_name", AuthSchema::getSchema());
    DBEntityVector* res = dbmgr.Search(cerca,false);
    if(verbose) cout << "AuthSchema::checkDB: res.size=" << res->size() << endl;
    if(dbmgr.getErrorMessage().length()==0 && res->size()>0) {
        if(verbose) {
            cout << "Results (" << typeid(res).name() << "):" << endl;
            for(const auto& elem : (*res)) {
                cout << "- " << elem->toString() << endl;
            }
        }
        dbecurrentversion = (DBEDBVersion*) res->at(0);
        res->clear();
        if(verbose) cout << "AuthSchema::checkDB: " << dbecurrentversion->toString("\n") << endl;
        current_db_version = dbecurrentversion->getIntegerValue("version");
    } else if(dbmgr.getErrorMessage().length()>0) {
        cerr << "AuthSchema::checkDB: " << dbmgr.getErrorMessage() << endl;
    }
    dbmgr.Destroy(res);
    delete cerca;

    if(verbose) cout << "AuthSchema::checkDB: current_db_version=" << current_db_version << endl;

    // Lambda :-)
    std::function<string(const string&)> lambda_dbeType2dbType = [&dbmgr] (const string& s) mutable -> string { return dbmgr.getConnection()->dbeType2dbType(s); };
    std::function<string(const string&)> lambda_getClazzSchema = [&dbmgr] (const string& s) mutable -> string {
        DBEntity* dbe = dbmgr.getClazz(s);
        string ret = dbe->getSchemaName();
        delete dbe;
        return ret;
    };

    // drop table dblayer_dbversion,auth_groups,auth_users,auth_users_groups;

    // 2. Do the DB migration
    long current_migration = -1;
    if(current_db_version<0) {
        string sql;
        bool use_fk = dbmgr.getConnection()->getDBType()!="MYSQL";// && dbmgr.getConnection()->getDBType()!="SQLite";
        DBEDBVersion dbversion;
        sql = dbversion.toSql(lambda_dbeType2dbType,lambda_getClazzSchema,"\n",use_fk);
        dbmgr.getConnection()->exec(sql);
        if(verbose) cout << sql << endl;

        dbecurrentversion = (AuthSchema::DBEDBVersion*) dbmgr.getClazzByTypeName("DBEDBVersion");
        dbecurrentversion->setValue("model_name", AuthSchema::getSchema());
        dbecurrentversion->setValue("version",current_db_version);
        dbecurrentversion = (AuthSchema::DBEDBVersion*) dbmgr.Insert(dbecurrentversion);
        if(verbose) cout << dbecurrentversion->toString("\n") << endl;

        DBEGroup* dbegroup = new DBEGroup();
        sql = dbegroup->toSql(lambda_dbeType2dbType,lambda_getClazzSchema,"\n",use_fk);
        dbmgr.getConnection()->exec(sql);
        if(verbose) cout << sql << endl;

        DBEUser dbeuser;
        sql = dbeuser.toSql(lambda_dbeType2dbType,lambda_getClazzSchema,"\n",use_fk);
        dbmgr.getConnection()->exec(sql);
        if(verbose) cout << sql << endl;

        DBEUserGroup dbeusergroup;
        sql = dbeusergroup.toSql(lambda_dbeType2dbType,lambda_getClazzSchema,"\n",use_fk);
        dbmgr.getConnection()->exec(sql);
        if(verbose) cout << sql << endl;

        dbegroup->setValue("id","-1")->setValue("name","Admin")->setValue("description","System admins"); dbmgr.Insert(dbegroup);
        dbegroup->setValue("id","-2")->setValue("name","Users")->setValue("description","System users");  dbmgr.Insert(dbegroup);
        dbegroup->setValue("id","-3")->setValue("name","Guests")->setValue("description","System guests (read only)"); dbmgr.Insert(dbegroup);
        delete dbegroup;

        dbeuser.setValue("id","-4")->setValue("login","adm")->setValue("pwd","adm")->setValue("fullname","Administrator")->setValue("group_id","-1")->setValue("pwd_salt",""); dbmgr.Insert(&dbeuser);
        dbeuser.setValue("id","-5")->setValue("login","usr")->setValue("pwd","usr")->setValue("fullname","A User")->setValue("group_id","-2")->setValue("pwd_salt","");        dbmgr.Insert(&dbeuser);
        dbeuser.setValue("id","-6")->setValue("login","guest")->setValue("pwd","guest")->setValue("fullname","A Guest")->setValue("group_id","-3")->setValue("pwd_salt","");   dbmgr.Insert(&dbeuser);

        dbeusergroup.setValue("user_id","-4")->setValue("group_id","-1"); dbmgr.Insert(&dbeusergroup);
        dbeusergroup.setValue("user_id","-5")->setValue("group_id","-2"); dbmgr.Insert(&dbeusergroup);
        dbeusergroup.setValue("user_id","-6")->setValue("group_id","-3"); dbmgr.Insert(&dbeusergroup);
    }
    current_migration++;

    // 3. Write version to DB
    dbecurrentversion->setValue("version",current_migration);
    dbmgr.Update(dbecurrentversion);

    //dbecurrentversion->setValue("");
    delete dbecurrentversion;

    if(verbose) cout << "AuthSchema::checkDB: end." << endl;
}
