#pragma once
/***************************************************************************
**	datefield.h  v0.1.0 - 2012.03.19
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
** @version $Id: datefield.h $
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

#include <string>
using namespace std;

#include "field.h"

namespace SchemaNS {

    class DateField : public Field  {
      public:
		/** YYYY-MM-DD HH:MM:SS mmm */
        DateField(const string& nome, const string& valore);
        DateField(const string& nome, long seconds);
        DateField(const string& nome,
                long year, long month, long day,
                long hour, long minute, long seconds, long millis
            );
        virtual ~DateField();

        virtual Field* clone() const;
        virtual bool equals(const Field& field) const;

        void setValue(const string& valore);
        /** Since 01/01/1970 */
        void setValue(long seconds);
        //bool isValid();
        //long getDaysFor(long year);
        //long getDaysFor(long year, long month);
        virtual string toString() const;
		/** Convert to seconds since 01/01/1970 */
        long to_seconds() const;
		/** Convert to days since 01/01/1970 */
        long to_days() const;
        virtual Field* createNewInstance(const char* aName=0) const;
      protected:
        long year,month,day,hour,minute,seconds, millis;
      private:
        bool isValidDate(long y, long m, long d) const;
        bool isValidHour(long h, long m, long s, long millis) const;
    };
}
