/***************************************************************************
**	dblayer.h  v0.1.0 - 2006.05.04
**	---------------------------------
**
**	Author:		Roberto Rocco Angeloni.
**	email:		roberto@roccoangeloni.it
**	Comment:
**	To Do:
**	Future:
**	History:
**		v0.0.1 - 2002.11.05	First Old Version
**		v0.1.0 - 2006.05.04 Rewritten for the new framework
**
** @copyright &copy; 2011 by Roberto Rocco Angeloni <roberto@roccoangeloni.it>
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


#ifndef DBLAYER_H
#define DBLAYER_H

#include "importedPackages.h"


/**
  *@author Roberto Rocco Angeloni
  */
namespace DBLayer {
	using namespace std;

	class DLLEXPORT DBField;
	class DLLEXPORT DBEntity;
	class DLLEXPORT ForeignKey;
	typedef DLLEXPORT vector<DBField*>      DBFieldVector;
	typedef DLLEXPORT map<string,DBField*>  DBFieldMap;
	typedef DLLEXPORT vector<DBEntity*>     DBEntityVector;
	typedef DLLEXPORT map<string,DBEntity*> DBEntityMap;
	typedef DLLEXPORT vector<ForeignKey*>   ForeignKeyVector;

	typedef DLLEXPORT vector<int>    IntegerVector;
	typedef DLLEXPORT vector<string> StringVector;

	DLLEXPORT string lowerCase(string& s);
	DLLEXPORT string upperCase(string& s);
    DLLEXPORT string joinString( StringVector* vs, string* glue );
    DLLEXPORT StringVector& splitString(const string& s, char delim, StringVector& elems);
    /** trim from start */
    DLLEXPORT string& string_ltrim(string& s);
    /** trim from end */
    DLLEXPORT string& string_rtrim(string& s);
    /** trim from both ends */
    DLLEXPORT string& string_trim(string& s);
	DLLEXPORT string integer2string(long longValue);
	/**	Rimpiazza tutte le occorrenze di <b>f</b> con <b>r</b> in <b>s</b>. */
	DLLEXPORT string replaceAll(string s, string f, string r);

}

#endif
