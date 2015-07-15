#pragma once

/***************************************************************************
**	schema.h  v0.1.0 - 2012.03.19
**	-----------------------------------
**
**	Author:		Roberto Rocco Angeloni.
**	E-mail:		roberto@roccoangeloni.it
**	Comment:	Implementazione di connection e resultset per ODBC
**	To Do:		- ...
**	Future:
**	History:
**		v0.1.0 - 2006.05.26 Versione iniziale
**
** @copyright &copy; 2011-2015 by Roberto Rocco Angeloni <roberto@roccoangeloni.it>
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

    int getSchemiCreati();
    int getSchemiDistrutti();


    /**
     * Contenitore di field
     */
    class Schema {

      public:
        Schema();
        Schema(const string* nome);
        virtual ~Schema();

        /** compatibility */
        string name() const;
        string getName() const;

        virtual string toString(string prefix="", bool valuesAsAttributes=false) const;

        /** Returns a new instance */
        virtual Schema* createNewInstance() const;
        virtual Schema* createNewInstance(const char* aName) const;
        /** Create a copy of current schema */
        Schema* clone(Schema* newSchema=0) const;

        virtual bool equals(const Schema* right) const;
        friend bool operator==(const Schema& left, const Schema& right);

        Schema* addField(Field* field );
        Field* getField(string& field) const;
        Field* getField(const string& field) const;
        Field* getField(int i) const;
        /** Ritorna la posizione del field nel vettore */
        int getFieldIndex(const string& field) const;
        /** Ritorna i dbfields presenti nella dbe */
        int getFieldSize() const;

        /** Ritorna i nomi dei campi presenti nella DBE */
        StringVector getNames() const;
        /** Ritorna un dizionario nome:valore della DBE */
        FieldMap getValuesDictionary() const;
        Schema* setValuesDictionary(FieldMap fields);

        /** SE non presente, aggiunge un field dello stesso tipo di valore, ALTRIMENTI ne aggiorna il contenuto. */
        Schema* setValue(const string& fieldName, bool valore);
        /** SE non presente, aggiunge un field dello stesso tipo di valore, ALTRIMENTI ne aggiorna il contenuto. */
        Schema* setValue(const string& fieldName, float valore);
        /** SE non presente, aggiunge un field dello stesso tipo di valore, ALTRIMENTI ne aggiorna il contenuto. */
        Schema* setValue(const string& fieldName, long valore);
        /** SE non presente, aggiunge un field dello stesso tipo di valore, ALTRIMENTI ne aggiorna il contenuto. */
        Schema* setValue(const string& fieldName, const string& valore);
        /**
         * Looks like if you have "a quoted string", the evaluation ordewr is:
         * const char*
         * bool
         * std::string
         * so we need to patch it like this :-(
         * See: http://stackoverflow.com/questions/1149109/why-does-a-quoted-string-match-bool-method-signature-before-a-stdstring
         */
        Schema* setValue(const string& fieldName, const char* valore);
        /** SE non presente, aggiunge un field dello stesso tipo di valore, ALTRIMENTI ne aggiorna il contenuto. */
        Schema* setDateValue(const string& fieldName, const string& valore);

        /**
         * Ritorna il valore del field in argomento
         * L'esistenza del field deve essere prima testata con la funzione isNull
         */
        string getValue(const string& fieldName) const;
        long getIntegerValue(const string& fieldName) const;
        bool getBooleanValue(const string& fieldName) const;
        string getDateValue(const string& fieldName) const;

        /**
         * Ritorna true se il field non è presente oppure se è presente ma ha valore nullo
         */
        bool isNull(const string& fieldName) const;
        Schema* setNull(const string& fieldName);
        Schema* setNull(const string& fieldName, bool valore);

        static int getSchemiCreati();
        static int getSchemiDistrutti();
        static vector<Schema*> getCreatedSchema();

      protected:
        virtual Field* createNewField(const string& fieldName, bool valore) const;
        virtual Field* createNewField(const string& fieldName, float valore) const;
        virtual Field* createNewField(const string& fieldName, long valore) const;
        virtual Field* createNewField(const string& fieldName, const string& valore) const;
        virtual Field* createNewDateField(const string& fieldName, const string& valore) const;

        /** Schema name */
        string _name;
        /** Schema fields */
        FieldVector fields;

      private:
        string toString_nodes(string prefix) const;

        static vector<Schema*> createdSchema;
        static int schemiCreati;
        static int schemiDistrutti;
    };

    vector<Schema*> getCreatedSchema();

    typedef vector<Schema*>     SchemaVector;
    typedef map<string,Schema*> SchemaMap;

    bool operator==(const Schema& left, const Schema& right);

    string integer2string(long longValue);
    string float2string(float f);
}
