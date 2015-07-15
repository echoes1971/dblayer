#pragma once
/***************************************************************************
** @copyright &copy; 2011-2015 by Roberto Rocco Angeloni <roberto@roccoangeloni.it>
** @license http://opensource.org/licenses/lgpl-3.0.html GNU Lesser General Public License, version 3.0 (LGPLv3)
** @version $Id: datefield.h $
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
 ***************************************************************************/

#include "importedPackages.h"

#include "dbfield.h"

/**
 *@author Roberto Rocco Angeloni
 */

namespace DBLayer {

    class DateField : public DBField  {
      public:
		/** YYYY-MM-DD HH:MM:SS mmm */
        DateField(const string& nome, const string& valore);
        DateField(const string& nome, long seconds);
        DateField(const string& nome,
                long year, long month, long day,
                long hour, long minute, long seconds, long millis
            );
        virtual ~DateField();
        void setValue(const string& valore);
        /** Since 01/01/1970 */
        void setValue(long seconds);
        //bool isValid();
        long getDaysFor(long year) const;
        long getDaysFor(long year, long month) const;
        virtual string toString() const;
		/** Convert to seconds since 01/01/1970 */
        long to_seconds() const;
		/** Convert to days since 01/01/1970 */
        long to_days() const;
        virtual Field* createNewInstance(const char* aName=0) const;
      private:
        long year,month,day,hour,minute,seconds, millis;
        bool isValidDate(long y, long m, long d) const;
        bool isValidHour(long h, long m, long s, long millis) const;
    };
}
