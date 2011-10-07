/***************************************************************************
**	dbefactory.cpp  v0.1.0 - 2006.05.09
**	-----------------------------------
**
**	Author:		Roberto Rocco Angeloni.
**	email:		roberto@roccoangeloni.it
**	Comment:
**	To Do:
**	Future:
**	History:
**		v0.1.0 - 2006.05.09	Completata la prima versione.
**
** @copyright &copy; 2011 by Roberto Rocco Angeloni <roberto@roccoangeloni.it>
** @license http://opensource.org/licenses/lgpl-3.0.html GNU Lesser General Public License, version 3.0 (LGPLv3)
** @version $Id: dbefactory.cpp $
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

#include <cstdio>

#include "dbefactory.h"
using namespace DBLayer;

DBEFactory::DBEFactory(bool verbose) { this->_verbose=verbose; }
DBEFactory::~DBEFactory() {
    if(this->_verbose) printf("DBEFactory::~DBEFactory: start.\n");
    // Svuoto la cache
    DBEntityMap::iterator theIterator;
    for(theIterator = this->_cache.begin(); theIterator!=this->_cache.end(); theIterator++) {
        if(this->_verbose) printf("DBEFactory::~DBEFactory: cancello %s => %s\n",(*theIterator).first.c_str(), (*theIterator).second->name().c_str() );
        delete (*theIterator).second;
    }
    this->_cache.clear();
    if(this->_verbose) printf("DBEFactory::~DBEFactory: end.\n");
}

void DBEFactory::registerClass(string tablename, DBEntity* clazz) { this->registerClass(&tablename, clazz); }
void DBEFactory::registerClass(string* tablename, DBEntity* clazz) {
    string tmpIndex=(*tablename);
    // SE esiste gia => cancello la vecchia
    if(this->_cache.find(tmpIndex)!=this->_cache.end())
        delete this->_cache[tmpIndex];
    this->_cache[tmpIndex] = clazz;
}

DBEntity* DBEFactory::getClazz(const string* tablename) {
    string nomeTabella(tablename->c_str());
    if( this->_cache.find(nomeTabella) != this->_cache.end() ) {
        return this->_cache[nomeTabella]->createNewInstance();
    }
    return new DBEntity(tablename);
}

DBEntity* DBEFactory::getClazzByTypeName(string* typeName, bool caseSensitive) {
    for(DBEntityMap::iterator it = this->_cache.begin(); it!= this->_cache.end(); it++) {
        if (caseSensitive && (*it).second->name()== (*typeName) ) {
            return (*it).second->createNewInstance();
        }
    }
    return new DBEntity();
}

DBEntityVector DBEFactory::getRegisteredTypes() {
    DBEntityVector ret;
    for(DBEntityMap::iterator it = this->_cache.begin(); it!= this->_cache.end(); it++) {
        ret.push_back( (*it).second );
    }
    return ret;
}

string DBEFactory::toString(string prefix) {
    string ret;
    ret.append(prefix + "<DBEFactory>");
    for(DBEntityMap::iterator it = this->_cache.begin(); it!= this->_cache.end(); it++) {
        ret.append(prefix + " <clazz>");
        ret.append(prefix + "  <key>" + (*it).first + "</key>" );
        ret.append(prefix + "  <value>" +  (*it).second->name() + "</value>");
        ret.append(prefix + " </clazz>");
    }
    ret.append(prefix + "</DBEFactory>");
    return ret;
}
