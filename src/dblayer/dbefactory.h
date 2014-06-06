/***************************************************************************
**	dbefactory.h  v0.1.0 - 2012.03.19
**	-----------------------------------
**
**	Author:		Roberto Rocco Angeloni.
**	E-mail:		roberto@roccoangeloni.it
**	Comment:
**	TODO:		- metodo 'call'
**	Future:
**	History:
**		v0.1.0 - 2006.05.09 Completata la prima versione.
**
** @copyright &copy; 2011-2014 by Roberto Rocco Angeloni <roberto@roccoangeloni.it>
** @license http://opensource.org/licenses/lgpl-3.0.html GNU Lesser General Public License, version 3.0 (LGPLv3)
** @version $Id: dbefactory.h $
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

#ifndef DBEFACTORY_H
#define DBEFACTORY_H

#include "importedPackages.h"

#include "dbentity.h"
#include "dblayer.h"

namespace DBLayer {

    class DECLSPECIFIER DBEFactory {
      public:
        DBEFactory(bool verbose=false);
        virtual ~DBEFactory();

        void registerClass(string tablename, DBEntity* clazz);
        void registerClass(string* tablename, DBEntity* clazz);
        DBEntityVector getRegisteredTypes();
        DBEntity* getClazz(const string* tablename);
        DBEntity* getClazzByTypeName(string* typeName, bool caseSensitive=true);
        DBEntity* call(string* tablename, StringVector names, StringVector values);

        string toString(string prefix="\n");

      protected:
        DBEntityMap _cache;
	private:
        bool _verbose;
    };

}
#endif
