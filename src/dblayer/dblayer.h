#pragma once
/***************************************************************************
**	dblayer.h  v0.1.0 - 2012.03.19
**	-----------------------------------
**
**	Author:		Roberto Rocco Angeloni.
**	E-mail:		roberto@roccoangeloni.it
**	Comment:
**	To Do:
**	Future:
**	History:
**		v0.0.1 - 2002.11.05	First Old Version
**		v0.1.0 - 2006.05.04 Rewritten for the new framework
**
** @copyright &copy; 2011-2015 by Roberto Rocco Angeloni <roberto@roccoangeloni.it>
** @license http://opensource.org/licenses/lgpl-3.0.html GNU Lesser General Public License, version 3.0 (LGPLv3)
** @version $Id: dblayer.h $
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

#include "importedPackages.h"

#include "schema/classfactory.h"

/**
 * @author Roberto Rocco Angeloni
 */
namespace DBLayer {
    using namespace std;

    class DBField;
    class DBEntity;
    class ForeignKey;
    typedef vector<DBField*>      DBFieldVector;
    typedef map<string,DBField*>  DBFieldMap;
    typedef vector<DBEntity*>     DBEntityVector;
    typedef map<string,DBEntity*> DBEntityMap;
    typedef vector<ForeignKey>   ForeignKeyVector;

    typedef Factory<DBEntity> DBEFactory;

    typedef vector<int>    IntegerVector;
    typedef vector<string> StringVector;

    /* Name, type, null, key (PRI,MUL), default */
    typedef map<string,StringVector> ColumnDefinitions;

    string lowerCase(string& s);
    string upperCase(string& s);
    string capitalizeCase(string& s);
    string joinString( StringVector* vs, const string glue );
    string joinString( StringVector* vs, const string* glue );
    StringVector& splitString(const string& s, char delim, StringVector& elems);
    /** trim from start */
    string& string_ltrim(string& s);
    /** trim from end */
    string& string_rtrim(string& s);
    /** trim from both ends */
    string& string_trim(string& s);
    string integer2string(long longValue);
    /**	Rimpiazza tutte le occorrenze di <b>f</b> con <b>r</b> in <b>s</b>. */
    string replaceAll(string s, string f, string r);
}
