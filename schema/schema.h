/***************************************************************************
**	schema.h  v0.1.0 - 2006.06.01
**	-----------------------------
**
**	Author:		Roberto Rocco Angeloni.
**	email:		roberto@roccoangeloni.it
**	Copyright:	(c) 2006 by Roberto Rocco Angeloni.
**	Comment:	Implementazione di connection e resultset per ODBC
**	To Do:		- ...
**	Future:
**	History:
**		v0.1.0 - 2006.05.26 Versione iniziale
**
** @copyright &copy; 2011 by Roberto Rocco Angeloni <roberto@roccoangeloni.it>
** @license http://opensource.org/licenses/lgpl-3.0.html GNU Lesser General Public License, version 3.0 (LGPLv3)
** @version $Id: schema.h $
** @package rproject::schema
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

#ifndef SCHEMA_SCHEMA_H
#define SCHEMA_SCHEMA_H


#include <map>
#include <string>
#include <vector>
using namespace std;

#include "field.h"

/**
  * SchemaNS
  *
  * Basic schema definitions
  *
  *@author Roberto Rocco Angeloni
  */
namespace SchemaNS {

    typedef vector<int> IntegerVector;
    typedef vector<string> StringVector;

    typedef vector<Field*> FieldVector;
    typedef map<string,Field*>  FieldMap;
    typedef map<string,int>  FieldIndex;

    DECLSPECIFIER int getSchemiCreati();
    DECLSPECIFIER int getSchemiDistrutti();

    /**
     * Contenitore di field
     */
    class DECLSPECIFIER Schema {

      public:
        Schema();
        Schema(const string* nome);
        virtual ~Schema();

        string getName();

        virtual string toString(string prefix="", bool valuesAsAttributes=false);

        /** Returns a new instance */
        virtual Schema* createNewInstance(const char* aName=0);
        /** Create a copy of current schema */
        Schema* clone(Schema* newSchema=0);

        virtual bool equals(Schema* right);
        friend DECLSPECIFIER bool operator==(const Schema& left, const Schema& right);

        virtual Field* createNewField(const string* fieldName, bool valore);
        virtual Field* createNewField(const string* fieldName, float valore);
        virtual Field* createNewField(const string* fieldName, long valore);
        virtual Field* createNewField(const string* fieldName, const string* valore);
        virtual Field* createNewDateField(const string* fieldName, const string* valore);

        void addField( Field* field );
        Field* getField(string* field);
        Field* getField(const string* field);
        Field* getField(int i);
        /** Ritorna la posizione del field nel vettore */
        int getFieldIndex(const string* field);
        /** Ritorna i dbfields presenti nella dbe */
        int getFieldSize();

        /** Ritorna i nomi dei campi presenti nella DBE */
        StringVector getNames();
        /** Ritorna un dizionario nome:valore della DBE */
        FieldMap getValuesDictionary();

        /** SE non presente, aggiunge un field dello stesso tipo di valore, ALTRIMENTI ne aggiorna il contenuto. */
        void setValue(const string* fieldName, bool valore);
        /** SE non presente, aggiunge un field dello stesso tipo di valore, ALTRIMENTI ne aggiorna il contenuto. */
        void setValue(const string* fieldName, float valore);
        /** SE non presente, aggiunge un field dello stesso tipo di valore, ALTRIMENTI ne aggiorna il contenuto. */
        void setValue(const string* fieldName, long valore);
        /** SE non presente, aggiunge un field dello stesso tipo di valore, ALTRIMENTI ne aggiorna il contenuto. */
        void setValue(const string* fieldName, const string* valore);
        /** SE non presente, aggiunge un field dello stesso tipo di valore, ALTRIMENTI ne aggiorna il contenuto. */
        void setDateValue(const string* fieldName, const string* valore);

        /**
         * Ritorna il valore del field in argomento
         * L'esistenza del field deve essere prima testata con la funzione isNull
         */
        string getValue(const string* fieldName);
        string getDateValue(const string* fieldName);

        /**
         * Ritorna true se il field non è presente oppure se è presente ma ha valore nullo
         */
        bool isNull(const string* fieldName);
        void setNull(const string* fieldName);
        void setNull(const string* fieldName, bool valore);

        static int getSchemiCreati();
        static int getSchemiDistrutti();

      protected:
        /** Schema name */
        string name;
        /** Schema fields */
        FieldVector fields;

      private:
        string toString_nodes(string prefix);

        static int schemiCreati;
        static int schemiDistrutti;
    };

    typedef vector<Schema*>     SchemaVector;
    typedef map<string,Schema*> SchemaMap;

	DECLSPECIFIER bool operator==(const Schema& left, const Schema& right);

    string integer2string(long longValue);
    string float2string(float f);
}

#endif
