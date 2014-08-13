/**
 * cmschema.h
 */

#ifndef CM_SCHEMA_H
#define CM_SCHEMA_H

#include "dblayer/dblayer.h"
#include "dblayer/dbentity.h"
#include "dblayer/dbfield.h"
#include "dblayer/dbmgr.h"
using namespace DBLayer;
#include <string>
using namespace std;

namespace CMSchema {

  class DBEObject : public DBEntity {
    public:
    DBEObject();
      virtual ~DBEObject();
      virtual string name() const;
      virtual string getTableName() const;
      virtual DBFieldVector* getKeys() const;
      virtual DBEObject* createNewInstance() const;
    private:
      static const string nomiCampiChiave[];
      static StringField chiave1; // uuid
      static DBFieldVector chiavi;
      static DBFieldVector ___init_keys();
  };

  class DBECountry : public DBEntity {
    public:
    DBECountry();
      virtual ~DBECountry();
      virtual string name() const;
      virtual string getTableName() const;
      virtual DBFieldVector* getKeys() const;
      virtual DBECountry* createNewInstance() const;
    private:
      static const string nomiCampiChiave[];
      static StringField chiave1; // uuid
      static DBFieldVector chiavi;
      static DBFieldVector ___init_keys();
  };

  class DBECompany : public DBEObject {
    public:
    DBECompany();
      virtual ~DBECompany();
      virtual string name() const;
      virtual string getTableName() const;
      virtual DBFieldVector* getKeys() const;
      virtual DBECompany* createNewInstance() const;
    private:
      static const string nomiCampiChiave[];
      static StringField chiave1; // uuid
      static DBFieldVector chiavi;
      static DBFieldVector ___init_keys();
  };

  class DBEPeople : public DBEObject {
    public:
    DBEPeople();
      virtual ~DBEPeople();
      virtual string name() const;
      virtual string getTableName() const;
      virtual DBFieldVector* getKeys() const;
      virtual DBEPeople* createNewInstance() const;
    private:
      static const string nomiCampiChiave[];
      static StringField chiave1; // uuid
      static DBFieldVector chiavi;
      static DBFieldVector ___init_keys();
  };

  class DBEFile : public DBEObject {
    public:
    DBEFile();
      virtual ~DBEFile();
      virtual string name() const;
      virtual string getTableName() const;
      virtual DBFieldVector* getKeys() const;
      virtual DBEFile* createNewInstance() const;
    private:
      static const string nomiCampiChiave[];
      static StringField chiave1; // uuid
      static DBFieldVector chiavi;
      static DBFieldVector ___init_keys();
  };

  class DBEFolder : public DBEObject {
    public:
    DBEFolder();
      virtual ~DBEFolder();
      virtual string name() const;
      virtual string getTableName() const;
      virtual DBFieldVector* getKeys() const;
      virtual DBEFolder* createNewInstance() const;
    private:
      static const string nomiCampiChiave[];
      static StringField chiave1; // uuid
      static DBFieldVector chiavi;
      static DBFieldVector ___init_keys();
  };

  class DBELink : public DBEObject {
    public:
    DBELink();
      virtual ~DBELink();
      virtual string name() const;
      virtual string getTableName() const;
      virtual DBFieldVector* getKeys() const;
      virtual DBELink* createNewInstance() const;
    private:
      static const string nomiCampiChiave[];
      static StringField chiave1; // uuid
      static DBFieldVector chiavi;
      static DBFieldVector ___init_keys();
  };

  class DBENote : public DBEObject {
    public:
    DBENote();
      virtual ~DBENote();
      virtual string name() const;
      virtual string getTableName() const;
      virtual DBFieldVector* getKeys() const;
      virtual DBENote* createNewInstance() const;
    private:
      static const string nomiCampiChiave[];
      static StringField chiave1; // uuid
      static DBFieldVector chiavi;
      static DBFieldVector ___init_keys();
  };

  class DBEPage : public DBEObject {
    public:
    DBEPage();
      virtual ~DBEPage();
      virtual string name() const;
      virtual string getTableName() const;
      virtual DBFieldVector* getKeys() const;
      virtual DBEPage* createNewInstance() const;
    private:
      static const string nomiCampiChiave[];
      static StringField chiave1; // uuid
      static DBFieldVector chiavi;
      static DBFieldVector ___init_keys();
  };

  string getSchema();
  void registerClasses(DBEFactory* dbeFactory);
}

#endif