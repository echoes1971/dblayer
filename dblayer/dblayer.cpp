/***************************************************************************
**	dblayer.cpp  v0.0.1 - 2006.05.07
**	--------------------------------
**
**	Author:		Roberto Rocco Angeloni.
**	email:		roberto@roccoangeloni.it
**	Comment:
**	To Do:
**	Future:
**	History:
**		v0.0.1 - 2006.04.XX	...
**
** @copyright &copy; 2011 by Roberto Rocco Angeloni <roberto@roccoangeloni.it>
** @license http://opensource.org/licenses/lgpl-3.0.html GNU Lesser General Public License, version 3.0 (LGPLv3)
** @version $Id: dblayer.cpp $
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
#include <config.h>

#include "dblayer.h"
using namespace DBLayer;

string DBLayer::lowerCase(string& s) {
    char* buf = new char[s.length()];
    s.copy(buf, s.length());
    for(unsigned int i = 0; i < s.length(); i++)
        buf[i] = tolower(buf[i]);
    string r(buf, s.length());
    delete buf;
    return r;
}
string DBLayer::upperCase(string& s) {
    char* buf = new char[s.length()];
    s.copy(buf, s.length());
    for(unsigned int i = 0; i < s.length(); i++)
        buf[i] = toupper(buf[i]);
    string r(buf, s.length());
    delete buf;
    return r;
}
string DBLayer::capitalizeCase(string& s) {
    char* buf = new char[s.length()];
    s.copy(buf, s.length());
    if(s.length()>0) buf[0]=toupper(buf[0]);
    for(unsigned int i = 1; i < s.length(); i++)
        buf[i] = tolower(buf[i]);
    string r(buf, s.length());
    delete buf;
    return r;
}
string DBLayer::joinString( StringVector* vs, string* glue ) {
    string ret;
    int size = vs->size();
    for(int i=0; i<size; i++) {
        ret.append( vs->at(i) );
        if(i<(size-1))
            ret.append( *glue );
    }
    return ret;
}

vector<string>& DBLayer::splitString(const string& s, char delim, vector<string>& elems) {
    string tmp = s;
    string s_delim("");
    s_delim.append(1,delim);
    tmp.append( 1, delim );
    stringstream ss(tmp);
    string item;
    while(getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}
string& DBLayer::string_ltrim(string& s) {
  s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
  return s;
}
string& DBLayer::string_rtrim(std::string& s) {
  s.erase(std::find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(), s.end());
  return s;
}
string& DBLayer::string_trim(string& s) { return string_ltrim(string_rtrim(s)); }
string DBLayer::integer2string(long longValue) {
    int i = 50-2;
    char tmp[50];  tmp[50-2] = '0';  tmp[50-1] = '\0';
    if ( longValue<0 ) {
        string tmpString = "-";
        return tmpString.append( DBLayer::integer2string( -1 * longValue ) );
    }
    if (longValue>=0 && longValue<10) {
        tmp[i] = (char) ((int)longValue%10)+'0';
        i--;
    } else {
        for (i=50-2; i>=0 && longValue>0 ; i--) {
            tmp[i] = (char) ((int)longValue%10)+'0';
            longValue = longValue / 10;
        }
    }
    return (char*)&tmp[i+1];
}
string DBLayer::replaceAll(string s, string f, string r) {
#ifdef __i386__
    unsigned int found = (unsigned int) s.find(f);
#else
    unsigned long found = s.find(f);
#endif
  while(found != string::npos && found<s.length()) {
    s.replace(found, f.length(), r);
    found = (unsigned int) s.find(f, found+r.size());
  }
  return s;
}
