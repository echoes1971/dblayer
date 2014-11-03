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

class DBELog : public DBEntity {
  public:
    DBELog();
    virtual ~DBELog();
    virtual string &name() const;
    virtual string getTableName() const;
    virtual StringVector& getKeys() const;
    virtual DBELog* createNewInstance() const;
    virtual ColumnDefinitions getColumns() const;
    virtual StringVector& getColumnNames() const;

    virtual DBLayer::StringVector getOrderBy() const;
  private:
    static StringVector _column_order;
    static ColumnDefinitions _columns;
    static StringVector chiavi;
};

class DBECountry : public DBEntity {
  public:
    DBECountry();
    virtual ~DBECountry();
    virtual string& name() const;
    virtual string getTableName() const;
    virtual StringVector& getKeys() const;
    virtual DBECountry* createNewInstance() const;
    virtual ColumnDefinitions getColumns() const;
    virtual StringVector& getColumnNames() const;

    virtual DBLayer::StringVector getOrderBy() const;

    // Custom methods
    bool init_table(DBMgr* dbmgr, bool verbose=false);
  private:
    static StringVector _column_order;
    static ColumnDefinitions _columns;
    static StringVector chiavi;
};

class ObjectMgr;
class DBEObject : public DBEntity {
  public:
    DBEObject();
    virtual ~DBEObject();
    virtual string& name() const;
    virtual string getTableName() const;
    virtual StringVector& getKeys() const;
    virtual ForeignKeyVector& getFK() const;
    virtual DBEObject* createNewInstance() const;
    virtual ColumnDefinitions getColumns() const;
    StringVector& getColumnNames() const;
    virtual DBLayer::StringVector getOrderBy() const;

    // Custom methods
    string getId() const;
    DBEObject* setId(const string s);
    string getName() const;
    DBEObject* setName(const string s);
    string getDescription() const;
    DBEObject* setDescription(const string s);
    string getOwnerId() const;
    DBEObject* setOwnerId(const string s);
    string getGroupId() const;
    DBEObject* setGroupId(const string s);
    bool isDeleted() const;

    bool canRead(const string kind="") const;
    bool canWrite(const string kind="") const;
    bool canExecute(const string kind="") const;

    virtual DBEObject* setDefaultValues(ObjectMgr* dbmgr=0);

    virtual void _before_insert(DBMgr* dbmgr=0);
    virtual void _before_update(DBMgr* dbmgr=0);
    virtual void _before_delete(DBMgr* dbmgr=0);

  protected:
    static string _getTodayString();
  private:
    static StringVector chiavi;
    static ForeignKeyVector _fkv;
    static StringVector _column_order;
    static ColumnDefinitions _columns;
};

class ObjectMgr : public DBMgr {
  public:
    ObjectMgr();
    ObjectMgr(Connection* con, bool verbose=false);
    virtual ~ObjectMgr();

    bool canRead(const DBEObject& obj) const;
    bool canWrite(const DBEObject& obj) const;
    bool canExecute(const DBEObject& obj) const;

    DBEntityVector* Select(const string &tableName, const string &searchString);
    DBEntity* Insert(DBEntity *dbe);
    DBEntity* Update(DBEntity *dbe);
    DBEntity* Delete(DBEntity *dbe);
    //DBEntity* Copy(DBEntity *dbe);

    virtual string _buildSelectString(DBEntity *dbe, bool uselike=false, bool caseSensitive=true);

    DBEntityVector* Search(DBEntity* dbe, bool uselike=true,
                            bool caseSensitive=true, const string& orderBy="",
                            bool ignore_deleted=true, bool full_object=true
                          );

    DBEObject* objectById(const string id, const bool ignore_deleted=true);
    DBEObject* fullObjectById(const string id, const bool ignore_deleted=true);
    DBEObject* objectByName(const string name, const bool ignore_deleted=true);
    DBEObject* fullObjectByName(const string name, const bool ignore_deleted=true);
};

class DBECompany : public DBEObject {
  public:
    DBECompany();
    virtual ~DBECompany();
    virtual string& name() const;
    virtual string getTableName() const;
    virtual ForeignKeyVector& getFK() const;
    virtual DBECompany* createNewInstance() const;
    virtual ColumnDefinitions getColumns() const;
    virtual StringVector& getColumnNames() const;
  private:
    static ForeignKeyVector _fkv;
    static StringVector _column_order;
    static ColumnDefinitions _columns;
};

class DBEPeople : public DBEObject {
  public:
    DBEPeople();
    virtual ~DBEPeople();
    virtual string& name() const;
    virtual string getTableName() const;
    virtual ForeignKeyVector& getFK() const;
    virtual DBEPeople* createNewInstance() const;
    virtual ColumnDefinitions getColumns() const;
    virtual StringVector& getColumnNames() const;
  private:
    static ForeignKeyVector _fkv;
    static StringVector _column_order;
    static ColumnDefinitions _columns;
};

/**
 * Represents an event in a calendar
 * TODO to be developed further and tested
 */
class DBEEvent : public DBEObject {
  public:
    DBEEvent();
    virtual ~DBEEvent();
    virtual string& name() const;
    virtual string getTableName() const;
    virtual ForeignKeyVector& getFK() const;
    virtual DBEEvent* createNewInstance() const;
    virtual ColumnDefinitions getColumns() const;
    virtual StringVector& getColumnNames() const;

    // Custom methods

  private:
    static ForeignKeyVector _fkv;
    static StringVector _column_order;
    static ColumnDefinitions _columns;
};

class DBEFile : public DBEObject {
  public:
    DBEFile();
    virtual ~DBEFile();
    virtual string& name() const;
    virtual string getTableName() const;
    virtual ForeignKeyVector& getFK() const;
    virtual DBEFile* createNewInstance() const;
    virtual ColumnDefinitions getColumns() const;
    virtual StringVector& getColumnNames() const;

    // Custom methods
    DBEFile* setRootDirectory(const string& dir);
    string getRootDirectory() const;

    DBEFile* setFilename(const string& f);
    string getFilename() const;

    string createFilename(const string& aId="", const string& aFilename="") const;
    string createObjectPath(DBEFile* an_obj=0);
    string getFullpath(DBEFile* an_obj=0);

    /** Copy the source file to the destination fullpath */
    bool readFile(const string& src_file, bool move=false);

    virtual void _before_insert(DBMgr* dbmgr=0);
//     virtual void _before_update(DBMgr* dbmgr=0);
//     virtual void _before_delete(DBMgr* dbmgr=0);

  protected:
    /**
     * root directory where to store files
     */
    static string _root_directory;
  private:
    static ForeignKeyVector _fkv;
    static StringVector _column_order;
    static ColumnDefinitions _columns;

    void _inherith_father_root(ObjectMgr* objmgr);
    string _file_checksum(const string fullpath) const;
    string _mimetype(const string fullpath) const;
};

class DBEFolder : public DBEObject {
  public:
    DBEFolder();
    virtual ~DBEFolder();
    virtual string& name() const;
    virtual string getTableName() const;
    virtual ForeignKeyVector& getFK() const;
    virtual DBEFolder* createNewInstance() const;
    virtual ColumnDefinitions getColumns() const;
    virtual StringVector& getColumnNames() const;

    // Custom methods
    virtual DBEObject* setDefaultValues(ObjectMgr* objmgr);
//     virtual void _before_insert(DBMgr* dbmgr=0);
    virtual void _before_update(DBMgr* dbmgr=0);

  private:
    static ForeignKeyVector _fkv;
    static StringVector _column_order;
    static ColumnDefinitions _columns;

    void _inherith_father_root(ObjectMgr* objmgr);
};

class DBELink : public DBEObject {
  public:
    DBELink();
    virtual ~DBELink();
    virtual string& name() const;
    virtual string getTableName() const;
    virtual ForeignKeyVector& getFK() const;
    virtual DBELink* createNewInstance() const;
    virtual ColumnDefinitions getColumns() const;
    virtual StringVector& getColumnNames() const;
  private:
    static ForeignKeyVector _fkv;
    static StringVector _column_order;
    static ColumnDefinitions _columns;
};

class DBENote : public DBEObject {
  public:
    DBENote();
    virtual ~DBENote();
    virtual string& name() const;
    virtual string getTableName() const;
    virtual ForeignKeyVector& getFK() const;
    virtual DBENote* createNewInstance() const;
    virtual ColumnDefinitions getColumns() const;
    virtual StringVector& getColumnNames() const;
  private:
    static ForeignKeyVector _fkv;
    static StringVector _column_order;
    static ColumnDefinitions _columns;
};

class DBEPage : public DBEObject {
  public:
    DBEPage();
    virtual ~DBEPage();
    virtual string& name() const;
    virtual string getTableName() const;
    virtual ForeignKeyVector& getFK() const;
    virtual DBEPage* createNewInstance() const;
    virtual ColumnDefinitions getColumns() const;
    virtual StringVector& getColumnNames() const;
  private:
    static ForeignKeyVector _fkv;
    static StringVector _column_order;
    static ColumnDefinitions _columns;
};

  inline const string& getSchema() { static string schema("cm"); return schema; }
  void registerClasses(DBEFactory* dbeFactory);

  void checkDB(DBMgr& dbmgr, bool verbose=false);
}

#endif
