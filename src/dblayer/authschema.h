/**
 * authschema.h
 */

#ifndef AUTH_SCHEMA_H
#define AUTH_SCHEMA_H

#include "dblayer/dblayer.h"
#include "dblayer/dbentity.h"
#include "dblayer/dbfield.h"
#include "dblayer/dbmgr.h"
using namespace DBLayer;
#include <string>
using namespace std;

namespace AuthSchema {

  class DBEDBVersion : public DBEntity {
    public:
      DBEDBVersion();
      virtual ~DBEDBVersion();

      virtual ColumnDefinitions getColumns() const;

      virtual string name() const;
      virtual string getTableName() const;
      virtual DBFieldVector* getKeys() const;
      virtual DBEDBVersion* createNewInstance() const;
      virtual DBLayer::StringVector getOrderBy() const;

      int version();
    private:
      static ColumnDefinitions _columns;
      static const string nomiCampiChiave[];
      static IntegerField chiave1; // int
      static DBFieldVector chiavi;
      static DBFieldVector ___init_keys();
  };

  class DBEUser : public DBEntity {
    public:
      DBEUser();
      virtual ~DBEUser();
      virtual ColumnDefinitions getColumns() const;
      virtual string name() const;
      virtual string getTableName() const;
      virtual DBFieldVector* getKeys() const;
      virtual ForeignKeyVector& getFK() const;
      virtual DBEUser* createNewInstance() const;
      virtual DBLayer::StringVector getOrderBy() const;

      // Custom methods
      vector<map<string,string> > getDefaultEntries() const;

      inline bool checkNewPassword() { return true; }

      bool isRoot();

      virtual void _before_insert(DBMgr* dbmgr=0);
      virtual void _after_insert(DBMgr* dbmgr=0);
      virtual void _after_update(DBMgr* dbmgr=0);
      virtual void _after_delete(DBMgr* dbmgr=0);

      void _createGroup(DBMgr* dbmgr=0);
      void _deleteGroup(DBMgr* dbmgr=0);
      void _checkGroupAssociation(DBMgr* dbmgr=0);
    private:
      static ColumnDefinitions _columns;
      static ForeignKeyVector _fkv;
      static const string nomiCampiChiave[];
      static StringField chiave1; // uuid
      static DBFieldVector chiavi;
      static DBFieldVector ___init_keys();
  };

  class DBEGroup : public DBEntity {
    public:
      DBEGroup();
      virtual ~DBEGroup();
      virtual ColumnDefinitions getColumns() const;
      virtual string name() const;
      virtual string getTableName() const;
      virtual DBFieldVector* getKeys() const;
      virtual DBEGroup* createNewInstance() const;
      virtual DBLayer::StringVector getOrderBy() const;

      // Custom methods
      vector<map<string,string> > getDefaultEntries() const;

      virtual void _before_insert(DBMgr* dbmgr=0);
      virtual void _after_insert(DBMgr* dbmgr=0);
      virtual void _after_delete(DBMgr* dbmgr=0);

    private:
      static ColumnDefinitions _columns;
      static const string nomiCampiChiave[];
      static StringField chiave1; // uuid
      static DBFieldVector chiavi;
      static DBFieldVector ___init_keys();
  };

  class DBEUserGroup : public DBEntity {
    public:
      DBEUserGroup();
      virtual ~DBEUserGroup();
      virtual ColumnDefinitions getColumns() const;
      virtual string name() const;
      virtual string getTableName() const;
      virtual DBFieldVector* getKeys() const;
      virtual ForeignKeyVector& getFK() const;
      virtual DBEUserGroup* createNewInstance() const;
      virtual DBLayer::StringVector getOrderBy() const;

      // Custom methods
      vector<map<string,string> > getDefaultEntries() const;

    private:
      static ColumnDefinitions _columns;
      static ForeignKeyVector _fkv;
      static const string nomiCampiChiave[];
      static StringField chiave1; // uuid
      static StringField chiave2; // uuid
      static DBFieldVector chiavi;
      static DBFieldVector ___init_keys();
  };

  string getSchema();
  void registerClasses(DBEFactory* dbeFactory);

  void checkDB(DBMgr& dbmgr);
}

#endif
