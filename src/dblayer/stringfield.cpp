/***************************************************************************
** @copyright &copy; 2011-2014 by Roberto Rocco Angeloni <roberto@roccoangeloni.it>
** @license http://opensource.org/licenses/lgpl-3.0.html GNU Lesser General Public License, version 3.0 (LGPLv3)
** @version $Id: stringfield.cpp $
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

#include "stringfield.h"
using namespace DBLayer;

StringField::StringField(const string* nome) : DBField(nome) {
  type = DBField::STRING;
}
StringField::StringField(const string* nome,const string* valore) : DBField(nome) {
  type = DBField::STRING;
  this->setValue(valore);
}
StringField::~StringField() {}

void StringField::setValue(const string* valore) {
    if(this->stringValue!=0) { delete this->stringValue; this->stringValue=0; }
    const char* tmpValore = valore->c_str();
    this->stringValue = new string(tmpValore);
    if ( ((long)valore!=0) && valore->size()>0 )
        nullo = false;
}
const string* DBLayer::StringField::getValue(){ return this->stringValue; }

string DBLayer::StringField::toString() {
    string ret( this->getValue()->c_str() );
    return ret;
}

Field* StringField::createNewInstance(const char* aName) const {
    Field* ret=0;
    string myName=string();
    if( aName==0 ) {
        myName.append( this->getName() );
    } else {
        myName.append( aName );
    }
    ret = (Field*) new StringField(&myName);
    return ret;
}
