/**
 * dbschema.cpp
 */

#include "authschema.h"
using namespace AuthSchema;
#include "cmschema.h"
using namespace CMSchema;

#include <ctime>
#include <string>
using namespace std;

//*********************** DBELog: start.
DBFieldVector DBELog::chiavi = { new StringField(string("ip")), new StringField(string("data")) };
ColumnDefinitions DBELog::_columns;
StringVector DBELog::_column_order = {"ip","data","ora","count","url","note","note2"};
ColumnDefinitions DBELog::getColumns() const { return _columns; }
StringVector DBELog::getColumnNames() const { return _column_order; }
DBELog::DBELog() {
    this->tableName.clear();
    this->schemaName = CMSchema::getSchema();
    if(_columns.size()==0) {
        for(const pair<string,vector<string> > pair: DBEntity::getColumns()) {
            _columns[pair.first] = pair.second;
        }
        _columns["ip"] = vector<string> {"uuid","not null"};
        _columns["data"] = vector<string> {"date","not null default '0000-00-00'"};
        _columns["ora"] = vector<string> {"time","not null default '00:00:00'"};
        _columns["count"] = vector<string> {"int","not null default 0"};
        _columns["url"] = vector<string> {"varchar(255)","default null"};
        _columns["note"] = vector<string> {"varchar(255)","not null default ''"};
        _columns["note2"] = vector<string> {"text","not null"};
    }
}
DBELog::~DBELog() {}
string DBELog::name() const { return "DBELog"; }
string DBELog::getTableName() const { return "log"; }
DBFieldVector* DBELog::getKeys() const { return &chiavi; }
DBELog* DBELog::createNewInstance() const { return new DBELog(); }
DBLayer::StringVector DBELog::getOrderBy() const {
    static DBLayer::StringVector ret({"data desc", "ora desc"});
    return ret;
}
//*********************** DBELog: end.

//*********************** DBEObject: start.
DBFieldVector DBEObject::chiavi = { new StringField(string("id")) };
ForeignKeyVector DBEObject::_fkv;
ColumnDefinitions DBEObject::_columns;
StringVector DBEObject::_column_order = {
    "id","owner","group_id","permissions"
    ,"creator","creation_date"
    ,"last_modify","last_modify_date"
    ,"deleted_by","deleted_date"
    ,"father_id"
    ,"name","description"
};
DBEObject::DBEObject() {
    this->tableName.clear();
    this->schemaName = CMSchema::getSchema();
    if(_columns.size()==0) {
        for(const pair<string,vector<string> > pair: DBEntity::getColumns()) {
            _columns[pair.first] = pair.second;
        }
        _columns["id"] = vector<string> {"uuid","not null"};
        _columns["owner"] = vector<string> {"uuid","not null"};
        _columns["group_id"] = vector<string> {"uuid","not null"};

        _columns["permissions"] = vector<string> {"char(9)","not null default 'rwx------'"};
        _columns["creator"] = vector<string> {"uuid","not null"};
        _columns["creation_date"] = vector<string> {"datetime","default null"};
        _columns["last_modify"] = vector<string> {"uuid","not null"};
        _columns["last_modify_date"] = vector<string> {"datetime","default null"};
        _columns["deleted_by"] = vector<string> {"uuid","not null"};
        _columns["deleted_date"] = vector<string> {"datetime","not null default '0000-00-00 00:00:00'"};
        _columns["deleted_by"] = vector<string> {"uuid","not null"};
        _columns["father_id"] = vector<string> {"uuid","default null"};
        _columns["name"] = vector<string> {"varchar(255)","not null"};
        _columns["description"] = vector<string> {"text","not null"};
    }
    if(_fkv.size()==0) {
        for(const DBLayer::ForeignKey& fk : DBEntity::getFK()) { _fkv.push_back(fk); }
        _fkv.push_back(ForeignKey("owner","users","id"));
        _fkv.push_back(ForeignKey("group_id","groups","id"));
        _fkv.push_back(ForeignKey("creator","users","id"));
        _fkv.push_back(ForeignKey("last_modify","users","id"));
        _fkv.push_back(ForeignKey("deleted_by","users","id"));
        _fkv.push_back(ForeignKey("father_id",DBEObject::getTableName(),"id"));
    }
}
DBEObject::~DBEObject() {}
string DBEObject::name() const { return "DBEObject"; }
string DBEObject::getTableName() const { return "objects"; }
DBEObject* DBEObject::createNewInstance() const { return new DBEObject(); }
DBFieldVector* DBEObject::getKeys() const { return &chiavi; }
ForeignKeyVector& DBEObject::getFK() const { return _fkv; }
ColumnDefinitions DBEObject::getColumns() const { return _columns; }
StringVector DBEObject::getColumnNames() const { return _column_order; }
DBLayer::StringVector DBEObject::getOrderBy() const {
    static DBLayer::StringVector ret({"name"});
    return ret;
}

string DBEObject::_getTodayString() {
    time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
    string ret("");
//    $oggi = $oggi_array['year'] . "/"
//     . ( strlen($oggi_array['mon'])<2 ? "0" : "" ) . $oggi_array['mon'] . "/"
//     . ( strlen($oggi_array['mday'])<2 ? "0" : "" ) . $oggi_array['mday'] . " "
//     . ( strlen($oggi_array['hours'])<2 ? "0" : "" ) . $oggi_array['hours'] . ":"
//     . ( strlen($oggi_array['minutes'])<2 ? "0" : "" ) . $oggi_array['minutes'];
    string month = SchemaNS::integer2string((long) now->tm_mon+1); if(month.length()<2) month="0"+month;
    string day = SchemaNS::integer2string((long) now->tm_mday+1); if(day.length()<2) day="0"+day;
    string hour = SchemaNS::integer2string((long) now->tm_hour); if(hour.length()<2) hour="0"+hour;
    string minute = SchemaNS::integer2string((long) now->tm_min); if(minute.length()<2) minute="0"+minute;
    string secs = SchemaNS::integer2string((long) now->tm_sec); if(secs.length()<2) secs="0"+secs;
    ret.append(SchemaNS::integer2string((long) now->tm_year + 1900))
        .append("-").append(month)
        .append("-").append(SchemaNS::integer2string((long) now->tm_mday))
        .append(" ")
        .append(hour).append(":").append(minute).append(":").append(secs);
    return ret;
}
string DBEObject::getId() const { return this->getField("id")==0 || this->getField("id")->isNull() ? "" : *(this->getField("id")->getStringValue()); }
string DBEObject::getOwnerId() const { return this->getField("owner")==0 || this->getField("owner")->isNull() ? "" : *(this->getField("owner")->getStringValue()); }
string DBEObject::getGroupId() const { return this->getField("group_id")==0 || this->getField("group_id")->isNull() ? "" : *(this->getField("group_id")->getStringValue()); }
bool DBEObject::isDeleted() const { return this->getField("deleted_date")==0 || this->getField("deleted_date")->isNull() || *(this->getField("deleted_date")->getStringValue())=="0000-00-00 00:00:00" ? false : true; }
bool DBEObject::canRead(const string kind) const {
    string perms = this->getField("permissions")==0 || this->getField("permissions")->isNull() ? "" : *(this->getField("permissions")->getStringValue());
    if(!(perms.length()>0)) return true;
    if(kind=="U") {
        return perms.at(0+0)=='r';
    } else if(kind=="G") {
        return perms.at(0+3)=='r';
    } else {
        return perms.at(0+6)=='r';
    }
}
bool DBEObject::canWrite(const string kind) const {
    string perms = this->getField("permissions")==0 || this->getField("permissions")->isNull() ? "" : *(this->getField("permissions")->getStringValue());
    if(!(perms.length()>0)) return true;
    if(kind=="U") {
        return perms.at(1+0)=='w';
    } else if(kind=="G") {
        return perms.at(1+3)=='w';
    } else {
        return perms.at(1+6)=='w';
    }
}
bool DBEObject::canExecute(const string kind) const {
    string perms = this->getField("permissions")==0 || this->getField("permissions")->isNull() ? "" : *(this->getField("permissions")->getStringValue());
    if(!(perms.length()>0)) return true;
    if(kind=="U") {
        return perms.at(2+0)=='x';
    } else if(kind=="G") {
        return perms.at(2+3)=='x';
    } else {
        return perms.at(2+6)=='x';
    }
}
void DBEObject::setDefaultValues(ObjectMgr* dbmgr) {
    DBEUser* myuser = (DBEUser*) dbmgr->getDBEUser();
    if(myuser!=0) {
        if(this->getField("owner")==0 || this->getField("owner")->isNull() || this->getField("owner")->getStringValue()->length()==0)
            this->setValue("owner",myuser->getId());
        if(this->getField("group_id")==0 || this->getField("group_id")->isNull() || this->getField("group_id")->getStringValue()->length()==0)
            this->setValue("group_id",myuser->getStringValue("group_id"));
        this->setValue("creator",myuser->getId());
        this->setValue("last_modify",myuser->getId());
    }
    string today = this->_getTodayString();
    this->setValue("creation_date",today);
    this->setValue("last_modify_date",today);
    this->setValue("deleted_date","0000-00-00 00:00:00");

    if(this->getField("father_id")==0 || this->getField("father_id")->isNull()) {
        this->setValue("father_id","0"); // TODO verify this

        if(this->getField("fk_obj_id")!=0 && !this->getField("fk_obj_id")->isNull()) {
            DBEObject* fkobj = dbmgr->objectById(this->getStringValue("id"));
            if(fkobj!=0) {
                this->setValue("group_id",fkobj->getStringValue("group_id"));
                this->setValue("permissions",fkobj->getStringValue("permissions"));
                this->setValue("father_id",this->getStringValue("fk_obj_id"));
                delete fkobj;
            }
        }
    } else {
        DBEObject* father = dbmgr->objectById(this->getStringValue("id"));
        if(father!=0) {
            this->setValue("group_id",father->getStringValue("group_id"));
            this->setValue("permissions",father->getStringValue("permissions"));
            delete father;
        }
    }
}
void DBEObject::_before_insert(DBMgr* dbmgr) {
    string myid = dbmgr->getNextUuid(this);
    this->setValue("id",myid);
    this->setDefaultValues((ObjectMgr*) dbmgr);
}
void DBEObject::_before_update(DBMgr* dbmgr) {
    DBEUser* myuser = (DBEUser*) dbmgr->getDBEUser();
    if(myuser!=0) {
        this->setValue("last_modify",myuser->getId());
    }
    this->setValue("last_modify_date",this->_getTodayString());
}
void DBEObject::_before_delete(DBMgr* dbmgr) {
    if(this->isDeleted()) return;
    DBEUser* myuser = (DBEUser*) dbmgr->getDBEUser();
    if(myuser!=0) {
        this->setValue("deleted_by",myuser->getId());
    }
    this->setValue("deleted_date",this->_getTodayString());
}
//*********************** DBEObject: end.


//*********************** ObjectManager: start.
ObjectMgr::ObjectMgr() : DBMgr::DBMgr() {}
ObjectMgr::ObjectMgr(Connection* con, bool verbose) : DBMgr::DBMgr(con,verbose) {}
ObjectMgr::~ObjectMgr() {
    if( this->verbose ) { cout << "ObjectMgr::~ObjectMgr: start." << endl; }
    if( this->verbose ) { cout << "ObjectMgr::~ObjectMgr: end." << endl; }
}
bool ObjectMgr::canRead(const DBEObject& obj) const {
    bool ret = false;
    if(obj.canRead())
        ret = true;
    else if(obj.canRead("G") && this->hasGroup( obj.getGroupId() ) )
        ret = true;
    else if(obj.canRead("U")) {
        DBEUser* myuser = (DBEUser*) this->getDBEUser();
        if(myuser!=0 && myuser->getId()==obj.getOwnerId()) {
            ret = true;
        }
    }
    return ret;
}
bool ObjectMgr::canWrite(const DBEObject& obj) const {
    bool ret = false;
    DBEUser* myuser = (DBEUser*) this->getDBEUser();
    if(myuser!=0 && myuser->getId()==obj.getStringValue("creator")) {
        ret = true;
    } else {
        if(obj.canWrite())
            ret = true;
        else if(obj.canWrite("G") && this->hasGroup( obj.getGroupId() ) )
            ret = true;
        else if(obj.canWrite("U") && myuser!=0 && myuser->getId()==obj.getOwnerId())
            ret = true;
    }
    return ret;
}
bool ObjectMgr::canExecute(const DBEObject& obj) const {
    bool ret = false;
    DBEUser* myuser = (DBEUser*) this->getDBEUser();
    if(myuser!=0 && myuser->getId()==obj.getStringValue("creator")) {
        ret = true;
    } else {
        if(obj.canExecute())
            ret = true;
        else if(obj.canExecute("G") && this->hasGroup( obj.getGroupId() ) )
            ret = true;
        else if(obj.canExecute("U") && myuser!=0 && myuser->getId()==obj.getOwnerId())
            ret = true;
    }
    return ret;
}
DBEntityVector* ObjectMgr::Select(const string &tableName, const string &searchString) {
    DBEntityVector* tmp = DBMgr::Select(tableName, searchString);
    DBEUser* myuser = (DBEUser*) this->getDBEUser();
    if(myuser!=0 && myuser->isRoot())
        return tmp;

    DBEntityVector* ret = new DBEntityVector();
    for(DBEntity* dbe : (*tmp) ) {
        bool append = false;
        DBEObject* obj = dynamic_cast<DBEObject*>(dbe);
        if( obj!=0 ) {
            if(myuser!=0 && myuser->getId()==obj->getStringValue("creator")) {
                append = true;
            } else if( obj->isDeleted() ) {
                append = false;
            } else if(this->canRead(*obj)) {
                append = true;
            }
        } else {
            append = true;
        }
        if(append)
            ret->push_back(dbe);
        else
            delete dbe;
    }
    delete tmp;
    return ret;
}
DBEntity* ObjectMgr::Insert(DBEntity *dbe) {
    bool have_permission = true;
    DBEObject* obj = dynamic_cast<DBEObject*>(dbe);
    if( obj!=0 ) {
        have_permission = this->canWrite(*obj);
    }
    return have_permission ? DBMgr::Insert(dbe) : 0;
}
DBEntity* ObjectMgr::Update(DBEntity *dbe) {
    bool have_permission = true;
    DBEObject* obj = dynamic_cast<DBEObject*>(dbe);
    if( obj!=0 ) {
        have_permission = this->canWrite(*obj);
    }
    return have_permission ? DBMgr::Update(dbe) : 0;
}
DBEntity* ObjectMgr::Delete(DBEntity *dbe) {
    bool have_permission = true;
    DBEObject* obj = dynamic_cast<DBEObject*>(dbe);
    if( obj!=0 ) {
        obj = this->fullObjectById( obj->getId(), false );
        have_permission = this->canWrite(*obj);
    }
    if( have_permission ) {
        if( obj==0 || obj->isDeleted() ) {
            return DBMgr::Delete(dbe);
        } else {
            //this->connect();
            dbe->_before_delete( this );
            string sqlString = this->_buildUpdateString( dbe );
            if( this->verbose ) cout << "ObjectMgr.delete: sqlString = " << sqlString << endl;
            ResultSet* rs = this->getConnection()->exec( sqlString );
            delete rs;
            dbe->_after_delete( this );
            return dbe;
        }
    } else {
        return dbe;
    }
}
string ObjectMgr::_buildSelectString(DBEntity* dbe, bool uselike, bool caseSensitive) {
    if( this->verbose ) cout << "ObjectMgr::_buildSelectString: start." << endl;
    DBEObject* obj = dynamic_cast<DBEObject*>(dbe);
    if(obj==0 || obj->name()!="DBEObject") {
        if( this->verbose ) cout << "ObjectMgr::_buildSelectString: dbe is not a DBEObject" << endl;
        if( this->verbose ) cout << "ObjectMgr::_buildSelectString: end." << endl;
        return DBMgr::_buildSelectString(dbe, uselike, caseSensitive);
    }
    if( this->verbose ) cout << "ObjectMgr::_buildSelectString: TODO" << endl;
    string searchString = DBMgr::_buildSelectString(dbe, uselike, caseSensitive);
    DBEntityVector types = this->getDBEFactory()->getRegisteredTypes();
    StringVector q;
//     $tipi = $this->getFactory()->getRegisteredTypes();
//     $q = array();
//     foreach($tipi as $tablename=>$classname) {
//         $mydbe = $this->getInstance($classname); // 2011.04.04 eval("\$mydbe=new $classname;");
//         if(!is_a($mydbe,'DBEObject') || is_a($mydbe,"DBAssociation") || $mydbe->getTypeName()=='DBEObject') continue;
//         $mydbe->setValuesDictionary($dbe->getValuesDictionary());
//         $q[]=str_replace("select * ",
//                 "select '$classname' as classname,id,owner,group_id,permissions,creator,creation_date,last_modify,last_modify_date,father_id,name,description ",
//                 parent::_buildSelectString($mydbe,$uselike,$caseSensitive));
//     }
//     $searchString = implode($q, " union ");
//     return $searchString;
    if( this->verbose ) cout << "ObjectMgr::_buildSelectString: end." << endl;
    return searchString;
}
DBEObject* ObjectMgr::objectById(const string id, const bool ignore_deleted) const {
    // TODO
    cout << "ObjectMgr::objectById: TODO" << endl;
    return 0;
//    $tipi = $this->getFactory()->getRegisteredTypes();
//    $q = array();
//    foreach($tipi as $tablename=>$classname) {
//        $mydbe = $this->getInstance($classname); // 2011.04.04 eval("\$mydbe=new $classname;");
//        if($classname=='DBEObject' || !is_a($mydbe,'DBEObject') || is_a($mydbe,"DBAssociation")) continue;
//        $q[]="select '$classname' as classname,id,owner,group_id,permissions,creator,"
//                    ."creation_date,last_modify,last_modify_date,"
//                    ."deleted_by,deleted_date," // 2012.04.04
//                    ."father_id,name,description"
//                ." from ".$this->buildTableName($mydbe)
//                ." where id='".DBEntity::hex2uuid($id)."'"
//                .($ignore_deleted?" and deleted_date='0000-00-00 00:00:00'":'');
//    }
//    $searchString = implode($q, " union ");
//    if ($this->_verbose ) { printf("query: $searchString<br/>\n"); }
//    $lista = $this->select('DBEObject', "objects", $searchString);
//    return count($lista)==1 ? $lista[0] : null;
}
DBEObject* ObjectMgr::fullObjectById(const string id, const bool ignore_deleted) const {
    // TODO
    cout << "ObjectMgr::fullObjectById: TODO" << endl;
    return 0;
//     $myobj = $this->objectById($id,$a_ignore_deleted);
//     if($myobj===null) return null;
//     eval("\$cerca=new ".$myobj->getValue('classname')."();");
//     $cerca->setValue('id',$myobj->getValue('id'));
//     $lista = $this->search($cerca,0,false,null,$a_ignore_deleted);
//     if ($this->_verbose ) { printf("ObjectMgr.fullObjectById: lista=".count($lista)."<br/>\n"); }
//     return count($lista)==1 ? $lista[0] : null;
}
//*********************** ObjectManager: end.




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

void CMSchema::checkDB(DBMgr& dbmgr, bool verbose) {
    if(verbose) cout << "CMSchema::checkDB: start." << endl;

    if(dbmgr.getConnection()->isProxy()) {
        cerr << "CMSchema::checkDB: WARNING - remote databases cannot be checked." << endl;
        return;
    }
    if(!dbmgr.connect()) {
        cerr << "CMSchema::checkDB: ERROR - UNABLE TO CONNECT TO DB!!!" << endl;
        return;
    }

    // 1. Check app version
    long current_db_version = -1;
    DBEDBVersion* dbecurrentversion;
    DBEDBVersion* cerca = new DBEDBVersion();
    cerca->setValue("model_name", CMSchema::getSchema());
    DBEntityVector* res = dbmgr.Search(cerca,false);
    if(verbose) cout << "CMSchema::checkDB: res.size=" << res->size() << endl;
    if(dbmgr.getErrorMessage().length()==0 && res->size()>0) {
        if(verbose) {
            cout << "Results (" << typeid(res).name() << "):" << endl;
            for(const auto& elem : (*res)) {
                cout << "- " << elem->toString() << endl;
            }
        }
        dbecurrentversion = (DBEDBVersion*) res->at(0);
        res->clear();
        if(verbose) cout << "CMSchema::checkDB: " << dbecurrentversion->toString("\n") << endl;
        current_db_version = dbecurrentversion->getIntegerValue("version");
    } else if(dbmgr.getErrorMessage().length()>0) {
        cerr << "CMSchema::checkDB: " << dbmgr.getErrorMessage() << endl;
    }
    dbmgr.Destroy(res);
    delete cerca;

    if(verbose) cout << "CMSchema::checkDB: current_db_version=" << current_db_version << endl;

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
        dbecurrentversion->setValue("model_name", CMSchema::getSchema());
        dbecurrentversion->setValue("version",current_db_version);
        dbecurrentversion = (AuthSchema::DBEDBVersion*) dbmgr.Insert(dbecurrentversion);
        if(verbose) cout << dbecurrentversion->toString("\n") << endl;

        StringVector classes = {"log","objects","countrylist","companies",
                                "people","files","folders","links",
                                "notes","pages"};
        for(const string clazz : classes) {
            DBEntity* dbe = dbmgr.getClazz(clazz);
            sql = dbe->toSql(lambda_dbeType2dbType,lambda_getClazzSchema,"\n",use_fk);
            dbmgr.getConnection()->exec(sql);
            if(verbose) cout << sql << endl;
            dbmgr.Delete(dbe);

            if(clazz=="companies") break;
        }


//        dbegroup.setValue("id","-1")->setValue("name","Admin")->setValue("description","System admins"); dbmgr.Insert(&dbegroup);
//        dbegroup.setValue("id","-2")->setValue("name","Users")->setValue("description","System users");  dbmgr.Insert(&dbegroup);
//        dbegroup.setValue("id","-3")->setValue("name","Guests")->setValue("description","System guests (read only)"); dbmgr.Insert(&dbegroup);

    }
    current_migration++;

    // 3. Write version to DB
    dbecurrentversion->setValue("version",current_migration);
    dbmgr.Update(dbecurrentversion);

    //dbecurrentversion->setValue("");
    delete dbecurrentversion;

    if(verbose) cout << "CMSchema::checkDB: end." << endl;
}
