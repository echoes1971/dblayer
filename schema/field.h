/***************************************************************************
**	field.h  v0.1.0 - 2012.03.19
**	-----------------------------------
**
**	Author:		Roberto Rocco Angeloni.
**	E-mail:		roberto@roccoangeloni.it
**	Comment:	Field generico
**	To Do:		- ...
**	Future:
**	History:
**		v0.1.0 - 2006.05.26 Versione iniziale
**
** @copyright &copy; 2011-2012 by Roberto Rocco Angeloni <roberto@roccoangeloni.it>
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

#ifndef DECLSPECIFIER
# if defined(_WIN32) || defined(__CYGWIN__)
#  ifdef _WINDLL
#    define DECLSPECIFIER __declspec(dllexport)
#    define EXPIMP_TEMPLATE
#  else
#    define DECLSPECIFIER __declspec(dllimport)
#    define EXPIMP_TEMPLATE extern
#  endif
# else
#  define DECLSPECIFIER
# endif
#endif


#include <string>
using namespace std;


namespace SchemaNS {
	extern string integer2string(long longValue);
    extern string float2string(float f);

	DECLSPECIFIER int getFieldCreati();
	DECLSPECIFIER int getFieldDistrutti();

    class DECLSPECIFIER Field {
      public:
        Field();
        Field(const string* nome);
        virtual ~Field();

        virtual Field* createNewInstance(const char* aName=0) const;
        /** L'implementazione di default va bene per i tipi normali, eccetto le <b>date</b> */
        virtual Field* clone() const;

        /** L'implementazione di default va bene per i tipi normali, eccetto le <b>date</b> */
        virtual bool equals(const Field* field) const;

        inline string getName() const { return this->name; }
        inline int getType() const { return type; }
        string getTypeName() const;
        bool isBoolean() const;
        bool isInteger() const;
        bool isFloat() const;
        bool isDouble() const;
        bool isDate() const;
        bool isString() const;

        virtual string toString() const;

        const string* getStringValue() const;
        float getFloatValue() const;
        long getIntegerValue() const;
        bool getBooleanValue() const;

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
        void* getValue() const;

        /** @return the field value as a string */
        string getValueAsString() const;

        bool isNull() const;
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

        static long getDaysFor(long year);
        static long getDaysFor(long year, long month);
        static long to_days(long year, long month, long day);
        static long to_seconds(long hour, long minutes, long seconds, long days=0);

        static int getFieldCreati();
        static int getFieldDistrutti();

      protected:

        /** Field name */
        string name;
        /** Field type */
        int type;
        /** is null? */
        bool nullo;

        bool boolValue;
        long longValue;
        float floatValue;
        string* stringValue;
      private:
        static int fieldCreati;
        static int fieldDistrutti;
    };

    DECLSPECIFIER bool operator==(const Field& left, const Field& right);
}

#endif
