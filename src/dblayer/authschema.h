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

      virtual string name() const;
      virtual string getTableName() const;
      virtual StringVector getKeys() const;
      virtual DBEDBVersion* createNewInstance() const;
      virtual ColumnDefinitions getColumns() const;
      virtual StringVector getColumnNames() const;
      virtual DBLayer::StringVector getOrderBy() const;

      int version();
    private:
      static StringVector _column_order;
      static ColumnDefinitions _columns;
      static StringVector chiavi;
  };

  class DBEUser : public DBEntity {
    public:
      DBEUser();
      virtual ~DBEUser();
      virtual string name() const;
      virtual string getTableName() const;
      virtual StringVector getKeys() const;
      virtual ForeignKeyVector& getFK() const;
      virtual DBEUser* createNewInstance() const;
      virtual ColumnDefinitions getColumns() const;
      virtual StringVector getColumnNames() const;
      virtual DBLayer::StringVector getOrderBy() const;

      // Custom methods
      string getId() const;

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
      static StringVector chiavi;
      static ForeignKeyVector _fkv;
      static StringVector _column_order;
      static ColumnDefinitions _columns;
  };

  class DBEGroup : public DBEntity {
    public:
      DBEGroup();
      virtual ~DBEGroup();
      virtual string name() const;
      virtual string getTableName() const;
      virtual StringVector getKeys() const;
      virtual DBEGroup* createNewInstance() const;
      virtual ColumnDefinitions getColumns() const;
      virtual StringVector getColumnNames() const;
      virtual DBLayer::StringVector getOrderBy() const;

      // Custom methods
      vector<map<string,string> > getDefaultEntries() const;

      virtual void _before_insert(DBMgr* dbmgr=0);
      virtual void _after_insert(DBMgr* dbmgr=0);
      virtual void _after_delete(DBMgr* dbmgr=0);

    private:
      static StringVector _column_order;
      static ColumnDefinitions _columns;
      static StringVector chiavi;
  };

  class DBEUserGroup : public DBEntity {
    public:
      DBEUserGroup();
      virtual ~DBEUserGroup();
      virtual string name() const;
      virtual string getTableName() const;
      virtual StringVector getKeys() const;
      virtual ForeignKeyVector& getFK() const;
      virtual DBEUserGroup* createNewInstance() const;
      virtual ColumnDefinitions getColumns() const;
      virtual StringVector getColumnNames() const;
      virtual DBLayer::StringVector getOrderBy() const;

      // Custom methods
      vector<map<string,string> > getDefaultEntries() const;

    private:
      static StringVector chiavi;
      static ForeignKeyVector _fkv;
      static StringVector _column_order;
      static ColumnDefinitions _columns;
  };

  string getSchema();
  void registerClasses(DBEFactory* dbeFactory);

  void checkDB(DBMgr& dbmgr, bool verbose=false);
}

#endif
