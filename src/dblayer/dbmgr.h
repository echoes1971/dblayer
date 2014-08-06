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
** @copyright &copy; 2011-2014 by Roberto Rocco Angeloni <roberto@roccoangeloni.it>
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

#ifndef DBMGR_H
#define DBMGR_H

#include "importedPackages.h"

#include "dbconnection.h"
#include "dbentity.h"
#include "dbefactory.h"

namespace DBLayer {

class DECLSPECIFIER DBMgr {
    public:
        static const int MAX_ENTITY = 100;

        DBMgr();
        DBMgr(Connection* con, bool verbose=false);
        virtual ~DBMgr();

        bool connect();
        bool disconnect();

        string getErrorMessage();

        void setDBEFactory(DBEFactory* dbeFactory);
        void setVerbose(bool b);
        DBEFactory* getDBEFactory();
        DBEntityVector getRegisteredTypes();
        /** get clazz by table name */
        DBEntity* getClazz(string* typeName);
        DBEntity* getClazzByTypeName(const string *typeName);

        DBEntity* getDBEUser();
        void setDBEUser(DBEntity* dbe);

        ColumnDefinitions getColumnsForTable(const string& tablename);

        DBEntity* Insert(DBEntity *dbe);
        DBEntity* Update(DBEntity *dbe);
        DBEntity* Delete(DBEntity *dbe);
        DBEntity* Copy(DBEntity *dbe);

        DBEntityVector* Select(const string* tableName, const string* searchString);

        DBEntityVector* Search(DBEntity* dbe, bool uselike=true,
                                                        bool caseSensitive=true, const string* orderBy=new string("") );
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
        string getNextUuid(DBEntity* dbe);

        string ping();

        DBEntity* login(const string user,const string pwd);
        DBEntity* relogin();
//        def getUserGroupsList(self):
//            return self.user_groups_list
//        def setUserGroupsList(self,user_groups_list):
//            self.user_groups_list = user_groups_list
//        def hasGroup(self,group_id):
//            """ The logged user has the requested group_id ? """
//            return "%s"%group_id in self.user_groups_list
        void addGroup(const string& group_id);
//        def addGroup(self,group_id):
//            if not group_id in self.user_groups_list:
//                self.user_groups_list.append(group_id)

        /** Destroy the DBEntity of the list and the list itself! */
        static void Destroy(DBEntityVector* lista);

        void setConnection( Connection* _newVal);
        Connection* getConnection();

        void setSchema(string schema);
        string getSchema();

  protected:
    string name;
    string errorMessage;

    virtual string escapeString(string s);
    virtual string quoteDate(string s);
    virtual DBEntity* _before_insert(DBEntity* dbe);
    virtual DBEntity* _after_insert(DBEntity* dbe);
    virtual DBEntity* _before_update(DBEntity* dbe);
    virtual DBEntity* _after_update(DBEntity* dbe);
    virtual DBEntity* _before_delete(DBEntity* dbe);
    virtual DBEntity* _after_delete(DBEntity* dbe);
    virtual DBEntity* _before_copy(DBEntity* dbe);
    virtual DBEntity* _after_copy(DBEntity* dbe);

  private:
    Connection* con;
    bool verbose;
    DBEFactory* dbeFactory;
    string _schema;
    DBEntity* _dbeuser;
    vector<string> _user_groups_list;

    void _loadUserGroups();

    void rs2dbelist(ResultSet* res, string *nomeTabella, DBEntityVector* ret);

    string _buildTableName(DBEntity* dbe);
    string _buildKeysCondition(DBEntity* dbe);
    string _buildInsertString(DBEntity* dbe);
    string _buildUpdateString(DBEntity* dbe);
    string _buildDeleteString(DBEntity* dbe);
    string _buildSelectString(DBEntity* dbe, bool uselike=false, bool caseSensitive=true);
    StringVector _buildWhereCondition(DBEntity* dbe, bool uselike=false, bool caseSensitive=true);

};

}
#endif
