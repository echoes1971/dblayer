/***************************************************************************
**	field.h  v0.1.0 - 2006.05.26
**	----------------------------
**
**	Author:		Roberto Rocco Angeloni.
**	email:		roberto@roccoangeloni.it
**	Copyright:	(c) 2006 by Roberto Rocco Angeloni.
**	Comment:	Field generico
**	To Do:		- ...
**	Future:
**	History:
**		v0.1.0 - 2006.05.26 Versione iniziale
**
** @copyright &copy; 2011 by Roberto Rocco Angeloni <roberto@roccoangeloni.it>
** @license http://opensource.org/licenses/lgpl-3.0.html GNU Lesser General Public License, version 3.0 (LGPLv3)
** @version $Id: field.h $
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

#ifndef SCHEMA_FIELD_H
#define SCHEMA_FIELD_H

#ifndef DLLEXPORT
#if defined(_WIN32) || defined(__CYGWIN__)
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif
#endif

#ifndef DLLIMPORT
#if defined(_WIN32) || defined(__CYGWIN__)
#define DLLIMPORT __declspec(dllimport)
#else
#define DLLIMPORT
#endif
#endif


#include <string>
using namespace std;


namespace SchemaNS {

    extern string integer2string(long longValue); // Forward declaration
    extern string float2string(float f); // Forward declaration

DLLEXPORT int getFieldCreati();
DLLEXPORT int getFieldDistrutti();

    class DLLEXPORT Field {
      public:
        Field();
        Field(const string* nome);
        virtual ~Field();

        virtual Field* createNewInstance(const char* aName=0);
        /** L'implementazione di default va bene per i tipi normali, eccetto le <b>date</b> */
        virtual Field* clone();

        /** L'implementazione di default va bene per i tipi normali, eccetto le <b>date</b> */
        virtual bool equals(Field* field);

        // 2011.09.09: end.
        inline string getName() { return this->name; }
        inline int getType() { return type; }
//        string getName();
//        int getType();
        // 2011.09.09: end.
        string getTypeName();
        bool isBoolean();
        bool isInteger();
        bool isFloat();
        bool isDouble();
        bool isDate();
        bool isString();

        virtual string toString();

        const string* getStringValue();
        float getFloatValue();
        long getIntegerValue();
        bool getBooleanValue();

        void setDateValue(const string* valore);
        void setDateValue(time_t valore);
        void setStringValue(const string* valore);
        void setStringValue(string valore);
        void setFloatValue(float valore);
        void setIntegerValue(long valore);
        void setBooleanValue(bool valore);

        void setValue(bool value);
        void setValue(long value);
        void setValue(float value);
        void setValue(string value);

        void setValue(void* value);
        void* getValue();

        /** @return the field value as a string */
        string getValueAsString();

        bool isNull();
        void setNull();
        void setNull(bool valore);
        enum FieldKind {
          ANY=0,
          INTEGER,
          BOOLEAN,
          FLOAT,
          STRING,
          DATE
        };

		// Datetime static functions: start.
        static long getDaysFor(long year);
        static long getDaysFor(long year, long month);
        static long to_days(long year, long month, long day);
        static long to_seconds(long hour, long minutes, long seconds, long days=0);
        // Datetime static functions: end.

        static int getFieldCreati();
        static int getFieldDistrutti();

      protected:

        /** Nome del field. */
        string name;
        /** Tipo del campo. */
        int type;
        /**  */
        bool nullo;

        bool boolValue;
        long longValue;
        float floatValue;
        string* stringValue;
      private:
        static int fieldCreati;
        static int fieldDistrutti;
    };

    DLLEXPORT bool operator==(const Field& left, const Field& right);
}

#endif
