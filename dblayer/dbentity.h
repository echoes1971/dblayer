/***************************************************************************
**	dbentity.h  v0.1.2 - 2006.05.11
**	-------------------------------
**
**	Author:		Roberto Rocco Angeloni.
**	email:		roberto@roccoangeloni.it
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
** @copyright &copy; 2011 by Roberto Rocco Angeloni <roberto@roccoangeloni.it>
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
//#include "dblayer.h" RRA - 2010.03.31 - Spostato sopra
#include "foreignkey.h"

#include <schema/schema.h>
using namespace SchemaNS;

namespace DBLayer {
    class DBMgr;

    class DLLEXPORT DBEntity : public Schema {
        friend class DBMgr;
      public:
        DBEntity();
        DBEntity(const string* tableName);
        virtual ~DBEntity();

        /** Ritorna lo schema di appartenenza della tabella */
        virtual const string* getSchemaName();
        virtual string getTableName();
    //  virtual bool mapFromRS(ResultSet *rs);
        /** Class name */
        virtual string name();
        /**	valuesAsAttributes:
            se true, i valori vengono rappresentati come attributi
            se false, i valori vengono rappresentati come nodi
        */
        virtual string toString(string prefix="", bool valuesAsAttributes=false);

        /**	Ritorna una nuova istanza della DBE	*/
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

        /**	Ritorna i campi chiave	*/
        virtual DBFieldVector* getKeys();
        /**	Ritorna i NOMI dei campi chiave	*/
        StringVector getKeyNames();
        /**	true se il campo e' chiave	*/
        bool isKey(string nomeCampo);
        /**	Ritorna le foreign keys	*/
        virtual ForeignKeyVector* getFK();
        ForeignKeyVector getFKForTable(string tablename);
        /**	Reads the content of the referenced columns in the referenced table
        mapped in the given dbe	*/
        void readFKFrom(DBEntity* dbe);
        /**	Writes the content of the referenced columns in the referenced table
        mapped in the given dbe	and returns it	*/
        DBEntity* writeFKTo(DBEntity* dbemaster);
        /**	Dice se il field della dbe e' foreign key.
        Se specificato, dice se il field e' foreign key VERSO la tabella riferita	*/
        bool isFK(string field_name, string tabella_riferita="");
        /**	Elimino tutti i campi chiave presenti nella DBE
        Metodo di appoggio per la <b>copy</b> del DBMgr	*/
        void cleanKeyFields();

        /** Controlla tutti i campi chiave e ritorna true <=> sono tutti vuoti */
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

      private:
        string toString_nodes(string prefix);

    };

}

#endif
