#pragma once
/***************************************************************************
**	dbmgr.h  v0.1.4 - 2012.03.19
**	-----------------------------------
**
**	Author:		Roberto Rocco Angeloni.
**	E-mail:		roberto@roccoangeloni.it
**	Comment:
**	TODO:		- Testare <b>quoteDate</b> su insert, update e buildKeyCondition
**	Future:
**	History:
**		v0.0.1 - 2002.10.23	First Old Version
**		v0.1.0 - 2006.05.04 Rewritten for the new framework
**		v0.1.1 - 2006.05.09 Implementato
**					_buildKeysCondition, _buildInsertString
**		v0.1.2 - 2006.05.10
**					Insert
**		v0.1.3 - 2006.05.11
**					Update, _buildUpdateString, Delete, _buildDeleteString
**					Copy
**		v0.1.4 - 2006.05.22 Aggiunto e usato <b>quoteDate</b>
**
** @copyright &copy; 2011-2015 by Roberto Rocco Angeloni <roberto@roccoangeloni.it>
** @license http://opensource.org/licenses/lgpl-3.0.html GNU Lesser General Public License, version 3.0 (LGPLv3)
** @version $Id: dbmgr.h $
** @package rproject::dblayer
**
** Permission to use, copy, modify, and distribute this software for any
** purpose with or without fee is hereby granted, provided that the above
** copyright notice and this permission notice appear in all copies.
**
** THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
** WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
** MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
** ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
** WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
** ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
** OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
****************************************************************************/

#include "importedPackages.h"

#include "dbconnection.h"
#include "dbentity.h"

namespace DBLayer {

class DBMgr {
  public:
    static const int MAX_ENTITY = 100;

    DBMgr();
    DBMgr(Connection* con, bool verbose=false);
    virtual ~DBMgr();

    bool connect();
    bool disconnect();

    void setLogger(std::function<void(const string&, bool)> logger);
    void log(const string& s, const bool newline=true) const;

    string getErrorMessage() const;

    void setDBEFactory(DBEFactory* dbeFactory);
    void setVerbose(bool b);
    DBEFactory* getDBEFactory() const;
    DBEntityVector getRegisteredTypes() const;
    /** get clazz by table name */
    DBEntity* getClazz(const string &typeName) const;
    DBEntity* getClazzByTypeName(const string& typeName) const;

    DBEntity* getDBEUser() const;
    void setDBEUser(DBEntity* dbe);

    ColumnDefinitions getColumnsForTable(const string& tablename);

    DBEntity* Insert(DBEntity* dbe);
    DBEntity* Update(DBEntity* dbe);
    DBEntity* Delete(DBEntity* dbe);
    DBEntity* Copy(DBEntity* dbe);

    DBEntityVector* Select(const string &tableName, const string &searchString) const;

    DBEntityVector* Search(DBEntity *dbe, bool uselike=true,
                            bool caseSensitive=true, const string& orderBy="" );
    DBEntityVector* searchByKeys(DBEntity* dbe);
    bool exists(DBEntity* dbe);

//        def getNextId(self, dbe):
//            nomeTabella = "%s_seq_id" % ( self._schema )
//            tmp = self.select(nomeTabella, "select id from %s where name=''" % ( nomeTabella ) )
//            myid = 1
//            if len(tmp)>0:
//                myid = tmp[0].getValue('id') + 1
//                self.select(nomeTabella, "insert into %s (id,name) values (%s,'')" % (nomeTabella,myid) )
//            else:
//                self.select(nomeTabella, "update %s set id=%s where name=''" % (nomeTabella,myid) )
//            return myid
    string getNextUuid();

    string ping();

    DBEntity* login(const string user,const string pwd);
    DBEntity* relogin();
    bool isLoggedIn() const;

    /** Returns a string uniquely identifying the user and the connection */
    string getServerIDString() const;

    DBEntityVector* getListUserGroup() const;
    DBEntityVector* getListGroups() const;
//        def getUserGroupsList(self):
//            return self.user_groups_list
//        def setUserGroupsList(self,user_groups_list):
//            self.user_groups_list = user_groups_list
    bool hasGroup(const string& group_id) const;
    void addGroup(const string& group_id);

    /** Destroy the DBEntity of the list and the list itself! */
    static void Destroy(DBEntityVector* lista);

    void setConnection(Connection* _newVal);
    Connection* getConnection() const;

    void setSchema(const string schema);
    string getSchema() const;

    string buildTableName(const DBEntity* dbe) const;

  protected:
    string name;
    string errorMessage;

    bool verbose;

    virtual string escapeString(string s) const;
    virtual string quoteDate(string s) const;
    virtual DBEntity* _before_insert(DBEntity* dbe);
    virtual DBEntity* _after_insert(DBEntity* dbe);
    virtual DBEntity* _before_update(DBEntity* dbe);
    virtual DBEntity* _after_update(DBEntity* dbe);
    virtual DBEntity* _before_delete(DBEntity* dbe);
    virtual DBEntity* _after_delete(DBEntity* dbe);
    virtual DBEntity* _before_copy(DBEntity* dbe);
    virtual DBEntity* _after_copy(DBEntity* dbe);

    string _buildUpdateString(const DBEntity* dbe) const;
    virtual string _buildSelectString(DBEntity* dbe, bool uselike=false, bool caseSensitive=true);
    string _buildTableName(const DBEntity* dbe) const;

  private:
    Connection* con;
    DBEFactory* dbeFactory;
    string _schema;
    DBEntity* _dbeuser;
    /** This keeps only the IDs */
    StringVector _user_groups_list;
    /** This keeps the DBEUserGroup */
    DBEntityVector* _list_usergroups;
    /** This keeps the DBEGroups */
    DBEntityVector* _list_groups;

    std::function<void(const string&, bool)> _logger;

    void _loadUserGroups();

    void rs2dbelist(const ResultSet* res, const string& nomeTabella, DBEntityVector* ret) const;

    string _buildKeysCondition(const DBEntity* dbe) const;
    string _buildInsertString(const DBEntity* dbe) const;
    string _buildDeleteString(const DBEntity* dbe) const;
    StringVector _buildWhereCondition(const DBEntity* dbe, const bool uselike=false, const bool caseSensitive=true) const;

};

}
