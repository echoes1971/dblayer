/***************************************************************************
**	dbschema.cpp  v0.0.1 - 2006.05.04
**	---------------------------------
**
**	Author:		Roberto Rocco Angeloni.
**	email:		roberto@roccoangeloni.it
**	Copyright:	(c) 2011 by Roberto Rocco Angeloni.
**	Comment:
**	To Do:
**	Future:
**	History:
**		v0.0.1 - 2006.04.XX	...
****************************************************************************/

#include "dbschema.h"
using namespace MySchema;

#include <string>
using namespace std;

//*********************** DBEDBVersion: start.
const string DBEDBVersion::nomiCampiChiave[] = { string("version") };
IntegerField DBEDBVersion::chiave1( (const string*)&DBEDBVersion::nomiCampiChiave[0] );
DBFieldVector DBEDBVersion::chiavi = DBEDBVersion::___init_keys();
DBFieldVector DBEDBVersion::___init_keys() { DBFieldVector ret = DBFieldVector(); ret.push_back( &DBEDBVersion::chiave1 ); return ret; }
DBEDBVersion::DBEDBVersion() { this->tableName.clear(); }
DBEDBVersion::~DBEDBVersion() {}
string DBEDBVersion::name() { return "DBEDBVersion"; }
string DBEDBVersion::getTableName() { return "dbversion"; }
DBFieldVector* DBEDBVersion::getKeys() { return &DBEDBVersion::chiavi; }
DBEDBVersion* DBEDBVersion::createNewInstance() { return new DBEDBVersion(); }
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
StringField DBEGroup::chiave1( (const string*)&DBEGroup::nomiCampiChiave[0] );
DBFieldVector DBEGroup::chiavi = DBEGroup::___init_keys();
DBFieldVector DBEGroup::___init_keys() { DBFieldVector ret = DBFieldVector(); ret.push_back( &DBEGroup::chiave1 ); return ret; }
DBEGroup::DBEGroup() { this->tableName.clear(); }
DBEGroup::~DBEGroup() {}
string DBEGroup::name() { return "DBEGroup"; }
string DBEGroup::getTableName() { return "groups"; }
DBFieldVector* DBEGroup::getKeys() { return &DBEGroup::chiavi; }
DBEGroup* DBEGroup::createNewInstance() { return new DBEGroup(); }
//*********************** DBEGroup: end.

//*********************** DBEUserGroup: start.
const string DBEUserGroup::nomiCampiChiave[] = { string("user_id"), string("group_id") };
StringField DBEUserGroup::chiave1( (const string*)&DBEUserGroup::nomiCampiChiave[0] );
StringField DBEUserGroup::chiave2( (const string*)&DBEUserGroup::nomiCampiChiave[1] );
DBFieldVector DBEUserGroup::chiavi = DBEUserGroup::___init_keys();
DBFieldVector DBEUserGroup::___init_keys() { DBFieldVector ret = DBFieldVector(); ret.push_back( &DBEUserGroup::chiave1 ); ret.push_back( &DBEUserGroup::chiave2 ); return ret; }
DBEUserGroup::DBEUserGroup() { this->tableName.clear(); }
DBEUserGroup::~DBEUserGroup() {}
string DBEUserGroup::name() { return "DBEUserGroup"; }
string DBEUserGroup::getTableName() { return "users_groups"; }
DBFieldVector* DBEUserGroup::getKeys() { return &DBEUserGroup::chiavi; }
DBEUserGroup* DBEUserGroup::createNewInstance() { return new DBEUserGroup(); }
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

//*********************** DBEObject: start.
const string DBEObject::nomiCampiChiave[] = { string("id") };
StringField DBEObject::chiave1( (const string*)&DBEObject::nomiCampiChiave[0] );
DBFieldVector DBEObject::chiavi = DBEObject::___init_keys();
DBFieldVector DBEObject::___init_keys() { DBFieldVector ret = DBFieldVector(); ret.push_back( &DBEObject::chiave1 ); return ret; }
DBEObject::DBEObject() { this->tableName.clear(); }
DBEObject::~DBEObject() {}
string DBEObject::name() { return "DBEObject"; }
string DBEObject::getTableName() { return "objects"; }
DBFieldVector* DBEObject::getKeys() { return &DBEObject::chiavi; }
DBEObject* DBEObject::createNewInstance() { return new DBEObject(); }
//*********************** DBEObject: end.

//*********************** DBECountry: start.
const string DBECountry::nomiCampiChiave[] = { string("id") };
StringField DBECountry::chiave1( (const string*)&DBECountry::nomiCampiChiave[0] );
DBFieldVector DBECountry::chiavi = DBECountry::___init_keys();
DBFieldVector DBECountry::___init_keys() { DBFieldVector ret = DBFieldVector(); ret.push_back( &DBECountry::chiave1 ); return ret; }
DBECountry::DBECountry() { this->tableName.clear(); }
DBECountry::~DBECountry() {}
string DBECountry::name() { return "DBECountry"; }
string DBECountry::getTableName() { return "countrylist"; }
DBFieldVector* DBECountry::getKeys() { return &DBECountry::chiavi; }
DBECountry* DBECountry::createNewInstance() { return new DBECountry(); }
//*********************** DBECountry: end.

//*********************** DBECompany: start.
const string DBECompany::nomiCampiChiave[] = { string("id") };
StringField DBECompany::chiave1( (const string*)&DBECompany::nomiCampiChiave[0] );
DBFieldVector DBECompany::chiavi = DBECompany::___init_keys();
DBFieldVector DBECompany::___init_keys() { DBFieldVector ret = DBFieldVector(); ret.push_back( &DBECompany::chiave1 ); return ret; }
DBECompany::DBECompany() { this->tableName.clear(); }
DBECompany::~DBECompany() {}
string DBECompany::name() { return "DBECompany"; }
string DBECompany::getTableName() { return "companies"; }
DBFieldVector* DBECompany::getKeys() { return &DBECompany::chiavi; }
DBECompany* DBECompany::createNewInstance() { return new DBECompany(); }
//*********************** DBECompany: end.

//*********************** DBEPeople: start.
const string DBEPeople::nomiCampiChiave[] = { string("id") };
StringField DBEPeople::chiave1( (const string*)&DBEPeople::nomiCampiChiave[0] );
DBFieldVector DBEPeople::chiavi = DBEPeople::___init_keys();
DBFieldVector DBEPeople::___init_keys() { DBFieldVector ret = DBFieldVector(); ret.push_back( &DBEPeople::chiave1 ); return ret; }
DBEPeople::DBEPeople() { this->tableName.clear(); }
DBEPeople::~DBEPeople() {}
string DBEPeople::name() { return "DBEPeople"; }
string DBEPeople::getTableName() { return "people"; }
DBFieldVector* DBEPeople::getKeys() { return &DBEPeople::chiavi; }
DBEPeople* DBEPeople::createNewInstance() { return new DBEPeople(); }
//*********************** DBEPeople: end.

//*********************** DBEProject: start.
const string DBEProject::nomiCampiChiave[] = { string("id") };
StringField DBEProject::chiave1( (const string*)&DBEProject::nomiCampiChiave[0] );
DBFieldVector DBEProject::chiavi = DBEProject::___init_keys();
DBFieldVector DBEProject::___init_keys() { DBFieldVector ret = DBFieldVector(); ret.push_back( &DBEProject::chiave1 ); return ret; }
DBEProject::DBEProject() { this->tableName.clear(); }
DBEProject::~DBEProject() {}
string DBEProject::name() { return "DBEProject"; }
string DBEProject::getTableName() { return "projects"; }
DBFieldVector* DBEProject::getKeys() { return &DBEProject::chiavi; }
DBEProject* DBEProject::createNewInstance() { return new DBEProject(); }
//*********************** DBEProject: end.

//*********************** DBEProjectCompanyRole: start.
const string DBEProjectCompanyRole::nomiCampiChiave[] = { string("id") };
StringField DBEProjectCompanyRole::chiave1( (const string*)&DBEProjectCompanyRole::nomiCampiChiave[0] );
DBFieldVector DBEProjectCompanyRole::chiavi = DBEProjectCompanyRole::___init_keys();
DBFieldVector DBEProjectCompanyRole::___init_keys() { DBFieldVector ret = DBFieldVector(); ret.push_back( &DBEProjectCompanyRole::chiave1 ); return ret; }
DBEProjectCompanyRole::DBEProjectCompanyRole() { this->tableName.clear(); }
DBEProjectCompanyRole::~DBEProjectCompanyRole() {}
string DBEProjectCompanyRole::name() { return "DBEProjectCompanyRole"; }
string DBEProjectCompanyRole::getTableName() { return "projects_companies_roles"; }
DBFieldVector* DBEProjectCompanyRole::getKeys() { return &DBEProjectCompanyRole::chiavi; }
DBEProjectCompanyRole* DBEProjectCompanyRole::createNewInstance() { return new DBEProjectCompanyRole(); }
//*********************** DBEProjectCompanyRole: end.

//*********************** DBEProjectCompany: start.
const string DBEProjectCompany::nomiCampiChiave[] = { string("project_id"), string("company_id"), string("projects_companies_role_id") };
StringField DBEProjectCompany::chiave1( (const string*)&DBEProjectCompany::nomiCampiChiave[0] );
StringField DBEProjectCompany::chiave2( (const string*)&DBEProjectCompany::nomiCampiChiave[1] );
StringField DBEProjectCompany::chiave3( (const string*)&DBEProjectCompany::nomiCampiChiave[2] );
DBFieldVector DBEProjectCompany::chiavi = DBEProjectCompany::___init_keys();
DBFieldVector DBEProjectCompany::___init_keys() { DBFieldVector ret = DBFieldVector(); ret.push_back( &DBEProjectCompany::chiave1 ); ret.push_back( &DBEProjectCompany::chiave2 ); ret.push_back( &DBEProjectCompany::chiave3 ); return ret; }
DBEProjectCompany::DBEProjectCompany() { this->tableName.clear(); }
DBEProjectCompany::~DBEProjectCompany() {}
string DBEProjectCompany::name() { return "DBEProjectCompany"; }
string DBEProjectCompany::getTableName() { return "projects_companies"; }
DBFieldVector* DBEProjectCompany::getKeys() { return &DBEProjectCompany::chiavi; }
DBEProjectCompany* DBEProjectCompany::createNewInstance() { return new DBEProjectCompany(); }
//*********************** DBEProjectCompany: end.

//*********************** DBEProjectPeopleRole: start.
const string DBEProjectPeopleRole::nomiCampiChiave[] = { string("id") };
StringField DBEProjectPeopleRole::chiave1( (const string*)&DBEProjectPeopleRole::nomiCampiChiave[0] );
DBFieldVector DBEProjectPeopleRole::chiavi = DBEProjectPeopleRole::___init_keys();
DBFieldVector DBEProjectPeopleRole::___init_keys() { DBFieldVector ret = DBFieldVector(); ret.push_back( &DBEProjectPeopleRole::chiave1 ); return ret; }
DBEProjectPeopleRole::DBEProjectPeopleRole() { this->tableName.clear(); }
DBEProjectPeopleRole::~DBEProjectPeopleRole() {}
string DBEProjectPeopleRole::name() { return "DBEProjectPeopleRole"; }
string DBEProjectPeopleRole::getTableName() { return "projects_people_roles"; }
DBFieldVector* DBEProjectPeopleRole::getKeys() { return &DBEProjectPeopleRole::chiavi; }
DBEProjectPeopleRole* DBEProjectPeopleRole::createNewInstance() { return new DBEProjectPeopleRole(); }
//*********************** DBEProjectPeopleRole: end.

//*********************** DBEProjectPeople: start.
const string DBEProjectPeople::nomiCampiChiave[] = { string("project_id"), string("people_id"), string("projects_people_role_id") };
StringField DBEProjectPeople::chiave1( (const string*)&DBEProjectPeople::nomiCampiChiave[0] );
StringField DBEProjectPeople::chiave2( (const string*)&DBEProjectPeople::nomiCampiChiave[1] );
StringField DBEProjectPeople::chiave3( (const string*)&DBEProjectPeople::nomiCampiChiave[2] );
DBFieldVector DBEProjectPeople::chiavi = DBEProjectPeople::___init_keys();
DBFieldVector DBEProjectPeople::___init_keys() { DBFieldVector ret = DBFieldVector(); ret.push_back( &DBEProjectPeople::chiave1 ); ret.push_back( &DBEProjectPeople::chiave2 ); ret.push_back( &DBEProjectPeople::chiave3 ); return ret; }
DBEProjectPeople::DBEProjectPeople() { this->tableName.clear(); }
DBEProjectPeople::~DBEProjectPeople() {}
string DBEProjectPeople::name() { return "DBEProjectPeople"; }
string DBEProjectPeople::getTableName() { return "projects_people"; }
DBFieldVector* DBEProjectPeople::getKeys() { return &DBEProjectPeople::chiavi; }
DBEProjectPeople* DBEProjectPeople::createNewInstance() { return new DBEProjectPeople(); }
//*********************** DBEProjectPeople: end.

//*********************** DBEProjectProjectRole: start.
const string DBEProjectProjectRole::nomiCampiChiave[] = { string("id") };
StringField DBEProjectProjectRole::chiave1( (const string*)&DBEProjectProjectRole::nomiCampiChiave[0] );
DBFieldVector DBEProjectProjectRole::chiavi = DBEProjectProjectRole::___init_keys();
DBFieldVector DBEProjectProjectRole::___init_keys() { DBFieldVector ret = DBFieldVector(); ret.push_back( &DBEProjectProjectRole::chiave1 ); return ret; }
DBEProjectProjectRole::DBEProjectProjectRole() { this->tableName.clear(); }
DBEProjectProjectRole::~DBEProjectProjectRole() {}
string DBEProjectProjectRole::name() { return "DBEProjectProjectRole"; }
string DBEProjectProjectRole::getTableName() { return "projects_projects_roles"; }
DBFieldVector* DBEProjectProjectRole::getKeys() { return &DBEProjectProjectRole::chiavi; }
DBEProjectProjectRole* DBEProjectProjectRole::createNewInstance() { return new DBEProjectProjectRole(); }
//*********************** DBEProjectProjectRole: end.

//*********************** DBEProjectProject: start.
const string DBEProjectProject::nomiCampiChiave[] = { string("project_id"), string("project2_id"), string("projects_projects_role_id") };
StringField DBEProjectProject::chiave1( (const string*)&DBEProjectProject::nomiCampiChiave[0] );
StringField DBEProjectProject::chiave2( (const string*)&DBEProjectProject::nomiCampiChiave[1] );
StringField DBEProjectProject::chiave3( (const string*)&DBEProjectProject::nomiCampiChiave[2] );
DBFieldVector DBEProjectProject::chiavi = DBEProjectProject::___init_keys();
DBFieldVector DBEProjectProject::___init_keys() { DBFieldVector ret = DBFieldVector(); ret.push_back( &DBEProjectProject::chiave1 ); ret.push_back( &DBEProjectProject::chiave2 ); ret.push_back( &DBEProjectProject::chiave3 ); return ret; }
DBEProjectProject::DBEProjectProject() { this->tableName.clear(); }
DBEProjectProject::~DBEProjectProject() {}
string DBEProjectProject::name() { return "DBEProjectProject"; }
string DBEProjectProject::getTableName() { return "projects_projects"; }
DBFieldVector* DBEProjectProject::getKeys() { return &DBEProjectProject::chiavi; }
DBEProjectProject* DBEProjectProject::createNewInstance() { return new DBEProjectProject(); }
//*********************** DBEProjectProject: end.

//*********************** DBEFile: start.
const string DBEFile::nomiCampiChiave[] = { string("id") };
StringField DBEFile::chiave1( (const string*)&DBEFile::nomiCampiChiave[0] );
DBFieldVector DBEFile::chiavi = DBEFile::___init_keys();
DBFieldVector DBEFile::___init_keys() { DBFieldVector ret = DBFieldVector(); ret.push_back( &DBEFile::chiave1 ); return ret; }
DBEFile::DBEFile() { this->tableName.clear(); }
DBEFile::~DBEFile() {}
string DBEFile::name() { return "DBEFile"; }
string DBEFile::getTableName() { return "files"; }
DBFieldVector* DBEFile::getKeys() { return &DBEFile::chiavi; }
DBEFile* DBEFile::createNewInstance() { return new DBEFile(); }
//*********************** DBEFile: end.

//*********************** DBEFolder: start.
const string DBEFolder::nomiCampiChiave[] = { string("id") };
StringField DBEFolder::chiave1( (const string*)&DBEFolder::nomiCampiChiave[0] );
DBFieldVector DBEFolder::chiavi = DBEFolder::___init_keys();
DBFieldVector DBEFolder::___init_keys() { DBFieldVector ret = DBFieldVector(); ret.push_back( &DBEFolder::chiave1 ); return ret; }
DBEFolder::DBEFolder() { this->tableName.clear(); }
DBEFolder::~DBEFolder() {}
string DBEFolder::name() { return "DBEFolder"; }
string DBEFolder::getTableName() { return "folders"; }
DBFieldVector* DBEFolder::getKeys() { return &DBEFolder::chiavi; }
DBEFolder* DBEFolder::createNewInstance() { return new DBEFolder(); }
//*********************** DBEFolder: end.

//*********************** DBELink: start.
const string DBELink::nomiCampiChiave[] = { string("id") };
StringField DBELink::chiave1( (const string*)&DBELink::nomiCampiChiave[0] );
DBFieldVector DBELink::chiavi = DBELink::___init_keys();
DBFieldVector DBELink::___init_keys() { DBFieldVector ret = DBFieldVector(); ret.push_back( &DBELink::chiave1 ); return ret; }
DBELink::DBELink() { this->tableName.clear(); }
DBELink::~DBELink() {}
string DBELink::name() { return "DBELink"; }
string DBELink::getTableName() { return "links"; }
DBFieldVector* DBELink::getKeys() { return &DBELink::chiavi; }
DBELink* DBELink::createNewInstance() { return new DBELink(); }
//*********************** DBELink: end.

//*********************** DBENote: start.
const string DBENote::nomiCampiChiave[] = { string("id") };
StringField DBENote::chiave1( (const string*)&DBENote::nomiCampiChiave[0] );
DBFieldVector DBENote::chiavi = DBENote::___init_keys();
DBFieldVector DBENote::___init_keys() { DBFieldVector ret = DBFieldVector(); ret.push_back( &DBENote::chiave1 ); return ret; }
DBENote::DBENote() { this->tableName.clear(); }
DBENote::~DBENote() {}
string DBENote::name() { return "DBENote"; }
string DBENote::getTableName() { return "notes"; }
DBFieldVector* DBENote::getKeys() { return &DBENote::chiavi; }
DBENote* DBENote::createNewInstance() { return new DBENote(); }
//*********************** DBENote: end.

//*********************** DBEPage: start.
const string DBEPage::nomiCampiChiave[] = { string("id") };
StringField DBEPage::chiave1( (const string*)&DBEPage::nomiCampiChiave[0] );
DBFieldVector DBEPage::chiavi = DBEPage::___init_keys();
DBFieldVector DBEPage::___init_keys() { DBFieldVector ret = DBFieldVector(); ret.push_back( &DBEPage::chiave1 ); return ret; }
DBEPage::DBEPage() { this->tableName.clear(); }
DBEPage::~DBEPage() {}
string DBEPage::name() { return "DBEPage"; }
string DBEPage::getTableName() { return "pages"; }
DBFieldVector* DBEPage::getKeys() { return &DBEPage::chiavi; }
DBEPage* DBEPage::createNewInstance() { return new DBEPage(); }
//*********************** DBEPage: end.

//*********************** DBETimetrack: start.
const string DBETimetrack::nomiCampiChiave[] = { string("id") };
StringField DBETimetrack::chiave1( (const string*)&DBETimetrack::nomiCampiChiave[0] );
DBFieldVector DBETimetrack::chiavi = DBETimetrack::___init_keys();
DBFieldVector DBETimetrack::___init_keys() { DBFieldVector ret = DBFieldVector(); ret.push_back( &DBETimetrack::chiave1 ); return ret; }
DBETimetrack::DBETimetrack() { this->tableName.clear(); }
DBETimetrack::~DBETimetrack() {}
string DBETimetrack::name() { return "DBETimetrack"; }
string DBETimetrack::getTableName() { return "timetracks"; }
DBFieldVector* DBETimetrack::getKeys() { return &DBETimetrack::chiavi; }
DBETimetrack* DBETimetrack::createNewInstance() { return new DBETimetrack(); }
//*********************** DBETimetrack: end.

//*********************** DBETodo: start.
const string DBETodo::nomiCampiChiave[] = { string("id") };
StringField DBETodo::chiave1( (const string*)&DBETodo::nomiCampiChiave[0] );
DBFieldVector DBETodo::chiavi = DBETodo::___init_keys();
DBFieldVector DBETodo::___init_keys() { DBFieldVector ret = DBFieldVector(); ret.push_back( &DBETodo::chiave1 ); return ret; }
DBETodo::DBETodo() { this->tableName.clear(); }
DBETodo::~DBETodo() {}
string DBETodo::name() { return "DBETodo"; }
string DBETodo::getTableName() { return "todo"; }
DBFieldVector* DBETodo::getKeys() { return &DBETodo::chiavi; }
DBETodo* DBETodo::createNewInstance() { return new DBETodo(); }
//*********************** DBETodo: end.

//*********************** DBETodoTipo: start.
const string DBETodoTipo::nomiCampiChiave[] = { string("id") };
StringField DBETodoTipo::chiave1( (const string*)&DBETodoTipo::nomiCampiChiave[0] );
DBFieldVector DBETodoTipo::chiavi = DBETodoTipo::___init_keys();
DBFieldVector DBETodoTipo::___init_keys() { DBFieldVector ret = DBFieldVector(); ret.push_back( &DBETodoTipo::chiave1 ); return ret; }
DBETodoTipo::DBETodoTipo() { this->tableName.clear(); }
DBETodoTipo::~DBETodoTipo() {}
string DBETodoTipo::name() { return "DBETodoTipo"; }
string DBETodoTipo::getTableName() { return "todo_tipo"; }
DBFieldVector* DBETodoTipo::getKeys() { return &DBETodoTipo::chiavi; }
DBETodoTipo* DBETodoTipo::createNewInstance() { return new DBETodoTipo(); }
//*********************** DBETodoTipo: end.

//*********************** DBESocieta: start.
const string DBESocieta::nomiCampiChiave[] = { string("id") };
IntegerField DBESocieta::chiave1( (const string*)&DBESocieta::nomiCampiChiave[0] );
DBFieldVector DBESocieta::chiavi = DBESocieta::___init_keys();
DBFieldVector DBESocieta::___init_keys() { DBFieldVector ret = DBFieldVector(); ret.push_back( &DBESocieta::chiave1 ); return ret; }
DBESocieta::DBESocieta() { this->tableName.clear(); }
DBESocieta::~DBESocieta() {}
string DBESocieta::name() { return "DBESocieta"; }
string DBESocieta::getTableName() { return "societa"; }
DBFieldVector* DBESocieta::getKeys() { return &DBESocieta::chiavi; }
DBESocieta* DBESocieta::createNewInstance() { return new DBESocieta(); }
//*********************** DBESocieta: end.

//*********************** DBEMyLog: start.
const string DBEMyLog::nomiCampiChiave[] = { string("ip"), string("data") };
StringField DBEMyLog::chiave1( (const string*)&DBEMyLog::nomiCampiChiave[0] );
StringField DBEMyLog::chiave2( (const string*)&DBEMyLog::nomiCampiChiave[1] );
DBFieldVector DBEMyLog::chiavi = DBEMyLog::___init_keys();
DBFieldVector DBEMyLog::___init_keys() { DBFieldVector ret = DBFieldVector(); ret.push_back( &DBEMyLog::chiave1 ); ret.push_back( &DBEMyLog::chiave2 ); return ret; }
DBEMyLog::DBEMyLog() { this->tableName.clear(); }
DBEMyLog::~DBEMyLog() {}
string DBEMyLog::name() { return "DBEMyLog"; }
string DBEMyLog::getTableName() { return "mylog"; }
DBFieldVector* DBEMyLog::getKeys() { return &DBEMyLog::chiavi; }
DBEMyLog* DBEMyLog::createNewInstance() { return new DBEMyLog(); }
//*********************** DBEMyLog: end.

void MySchema::registerClasses(DBEFactory* dbeFactory) {
  dbeFactory->registerClass("dbversion", new DBEDBVersion() );
  dbeFactory->registerClass("users", new DBEUser() );
  dbeFactory->registerClass("groups", new DBEGroup() );
  dbeFactory->registerClass("users_groups", new DBEUserGroup() );
  dbeFactory->registerClass("log", new DBELog() );
  dbeFactory->registerClass("objects", new DBEObject() );
  dbeFactory->registerClass("countrylist", new DBECountry() );
  dbeFactory->registerClass("companies", new DBECompany() );
  dbeFactory->registerClass("people", new DBEPeople() );
  dbeFactory->registerClass("projects", new DBEProject() );
  dbeFactory->registerClass("projects_companies_roles", new DBEProjectCompanyRole() );
  dbeFactory->registerClass("projects_companies", new DBEProjectCompany() );
  dbeFactory->registerClass("projects_people_roles", new DBEProjectPeopleRole() );
  dbeFactory->registerClass("projects_people", new DBEProjectPeople() );
  dbeFactory->registerClass("projects_projects_roles", new DBEProjectProjectRole() );
  dbeFactory->registerClass("projects_projects", new DBEProjectProject() );
  dbeFactory->registerClass("files", new DBEFile() );
  dbeFactory->registerClass("folders", new DBEFolder() );
  dbeFactory->registerClass("links", new DBELink() );
  dbeFactory->registerClass("notes", new DBENote() );
  dbeFactory->registerClass("pages", new DBEPage() );
  dbeFactory->registerClass("timetracks", new DBETimetrack() );
  dbeFactory->registerClass("todo", new DBETodo() );
  dbeFactory->registerClass("todo_tipo", new DBETodoTipo() );
  dbeFactory->registerClass("societa", new DBESocieta() );
  dbeFactory->registerClass("mylog", new DBEMyLog() );
}
