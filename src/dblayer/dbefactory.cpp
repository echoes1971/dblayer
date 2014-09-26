/***************************************************************************
**	dbefactory.cpp  v0.1.0 - 2012.03.19
**	-----------------------------------
**
**	Author:		Roberto Rocco Angeloni.
**	E-mail:		roberto@roccoangeloni.it
**	Comment:
**	To Do:
**	Future:
**	History:
**		v0.1.0 - 2006.05.09	Completata la prima versione.
**
** @copyright &copy; 2011-2014 by Roberto Rocco Angeloni <roberto@roccoangeloni.it>
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
    // Cleaning the cache
    for(const auto& elem : this->_cache) {
        if(this->_verbose) printf("DBEFactory::~DBEFactory: deleting %s => %s\n",elem.first.c_str(), elem.second->name().c_str() );
        delete elem.second;
    }
    this->_cache.clear();
    if(this->_verbose) printf("DBEFactory::~DBEFactory: end.\n");
}

void DBEFactory::registerClass(const string& tablename, DBEntity* clazz) {
    string tmpIndex=tablename;
    // IF exists => delete the old definition
    if(this->_cache.find(tmpIndex)!=this->_cache.end())
        delete this->_cache[tmpIndex];
    this->_cache[tmpIndex] = clazz;
}

DBEntity* DBEFactory::getClazz(const string& tablename) const {
    string nomeTabella(tablename.c_str());
    if(this->_verbose) cout << "DBEFactory::getClazz: nomeTabella=" << nomeTabella << endl;
    if( this->_cache.find(nomeTabella) != this->_cache.end() ) {
        if(this->_verbose) cout << "DBEFactory::getClazz: found!" << endl;
        DBEntity* dbe = this->_cache.at(nomeTabella);
        return dbe->createNewInstance();
    }
    return new DBEntity(tablename);
}

DBEntity* DBEFactory::getClazzByTypeName(const string& typeName, bool caseSensitive) const {
    if(this->_verbose) cout << "DBEFactory::getClazzByTypeName: typeName=" << typeName << endl;
    for(const auto& elem : this->_cache) {
        if(this->_verbose) cout << " " << elem.first << ": " << elem.second->name() << endl;
        if(caseSensitive && elem.second->name()==typeName) {
            return elem.second->createNewInstance();
        }
    }
    return new DBEntity();
}

DBEntityVector DBEFactory::getRegisteredTypes() const {
    DBEntityVector ret;
    for(const auto& elem : this->_cache) {
        ret.push_back(elem.second);
    }
    return ret;
}

string DBEFactory::toString(string prefix) const {
    string ret;
    ret.append(prefix + "<DBEFactory>");
    for(const auto& elem : this->_cache) {
        ret.append(prefix + " <clazz>");
        ret.append(prefix + "  <key>" + elem.first + "</key>" );
        ret.append(prefix + "  <value>" +  elem.second->name() + "</value>");
        ret.append(prefix + " </clazz>");
    }
    ret.append(prefix + "</DBEFactory>");
    return ret;
}
