/**
 * dbschema.cpp
 */

#include "cmschema.h"
using namespace CMSchema;

#include <string>
using namespace std;

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
