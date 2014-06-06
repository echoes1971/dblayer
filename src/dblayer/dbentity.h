/***************************************************************************
**	dbentity.h  v0.1.2 - 2012.03.19
**	-----------------------------------
**
**	Author:		Roberto Rocco Angeloni.
**	E-mail:		roberto@roccoangeloni.it
**	Comment:
**	TODO:		- TESTARE la gestione delle FKs
**				- TESTARE getValuesDictionary, getDBFieldIndex
**				- Deprecare i metodi: void* getValue ?
**				- Implementare 'mapFromRS' ?
**	Future:
**	History:
**		v0.0.1 - 2002.10.20	First Old Version
**		v0.1.0 - 2006.05.04 Rewritten for the new framework
**		v0.1.1 - 2006.05.09 Implemented:
**					getFK, getFKForTable, readFKFrom, writeFKTo, isFK
**					getValuesDictionary, cleanKeyFields, getDBFieldIndex
**		v0.1.2 - 2006.05.11 Implemented:
**					getKeyNames, isKey, cleanKeyFields
**
** @copyright &copy; 2011-2014 by Roberto Rocco Angeloni <roberto@roccoangeloni.it>
** @license http://opensource.org/licenses/lgpl-3.0.html GNU Lesser General Public License, version 3.0 (LGPLv3)
** @version $Id: dbentity.h $
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


#ifndef DBENTITY_H
#define DBENTITY_H

#include "importedPackages.h"
#include "dblayer.h"
#include "dbfield.h"
#include "booleanfield.h"
#include "integerfield.h"
#include "floatfield.h"
#include "stringfield.h"
#include "datefield.h"
#include "foreignkey.h"

#include <schema/schema.h>
using namespace SchemaNS;

namespace DBLayer {
    class DECLSPECIFIER DBMgr;

    class DECLSPECIFIER DBEntity : public Schema {
        friend class DBMgr;
      public:
        DBEntity();
        DBEntity(const string* tableName);
        virtual ~DBEntity();

        string getColumnType(const string& column_name);
        ColumnDefinitions getColumns();
        static string dbeType2dbType(const string& dbetype);
        static string dbType2dbeType(const string& dbtype);
        static string dbConstraints2dbeConstraints(map<string,string>& def);
        static string dbColumnDefinition2dbeColumnDefinition(map<string,string>& def);

        /** Returns the column names of the default orderby */
        virtual DBLayer::StringVector getOrderBy() const;
        virtual string getOrderByString() const;

        static string uuid2hex(const string& str);
        static string hex2uuid(const string& a_str);

        /** Returns the table's own schema */
        virtual const string* getSchemaName();
        virtual string getTableName();
        /** Class name */
        virtual string name();
        /**
         * @param valuesAsAttributes
         *  if true, values are represented as attributes
         *  if false, values are represented as nodes
         */
        virtual string toString(string prefix="", bool valuesAsAttributes=false);

        /** @return a new DBE instance */
        virtual DBEntity* createNewInstance();
        virtual Field* createNewField(const string* fieldName, bool valore);
        virtual Field* createNewField(const string* fieldName, float valore);
        virtual Field* createNewField(const string* fieldName, long valore);
        virtual Field* createNewField(const string* fieldName, const string* valore);
        virtual Field* createNewDateField(const string* fieldName, const string* valore);

        /**	TODO: DEPRECARE?	*/
        void* getValue(string* field);
        /**	TODO: DEPRECARE?	*/
        void* getValue(const string& field);

        string getStringValue(const string* fieldName);

        /** @return the key fields */
        virtual DBFieldVector* getKeys() const;
        /** @return the key fields NAMES */
        StringVector getKeyNames() const;
        /** @return true if the field name is key */
        bool isKey(string fieldName);
        /** @return the foreign keys */
        virtual ForeignKeyVector* getFK();
        ForeignKeyVector getFKForTable(string tablename);
        /**	Reads the content of the referenced columns in the referenced table
        mapped in the given dbe	*/
        void readFKFrom(DBEntity* dbe);
        /**	Writes the content of the referenced columns in the referenced table
        mapped in the given dbe	and returns it	*/
        DBEntity* writeFKTo(DBEntity* dbemaster);
        /**	true if the given field name is foreign key.
        If specified, tells wheter is fk FOR the given table */
        bool isFK(string field_name, string referred_table="");
        /**	Destroy all the key fields in the given DBE
        Utility method for <b>DBMgr::copy</b>	*/
        void cleanKeyFields();

        /** @return true <=> all key fields are empty */
        bool isNew();

        virtual void _before_insert(DBMgr* dbmgr=0);
        virtual void _after_insert(DBMgr* dbmgr=0);

        virtual void _before_update(DBMgr* dbmgr=0);
        virtual void _after_update(DBMgr* dbmgr=0);

        virtual void _before_delete(DBMgr* dbmgr=0);
        virtual void _after_delete(DBMgr* dbmgr=0);

        virtual void _before_copy(DBMgr* dbmgr=0);
        virtual void _after_copy(DBMgr* dbmgr=0);

      protected:
        string tableName;

        /* column_name => array('type','constraints', ...) */
        static ColumnDefinitions _columns;

      private:
        string toString_nodes(string prefix);

    };

}

#endif