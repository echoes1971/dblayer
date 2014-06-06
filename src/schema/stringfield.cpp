/***************************************************************************
**	stringfield.cpp  v0.1.0 - 2012.03.19
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
** @copyright &copy; 2011-2014 by Roberto Rocco Angeloni <roberto@roccoangeloni.it>
** @license http://opensource.org/licenses/lgpl-3.0.html GNU Lesser General Public License, version 3.0 (LGPLv3)
** @version $Id: stringfield.cpp $
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

#include "stringfield.h"
using namespace SchemaNS;

StringField::StringField(const string* nome) : Field(nome){
    type = Field::STRING;
}
StringField::StringField(const string* nome,const string* valore) : Field(nome){
    type = Field::STRING;
    this->setValue( valore);
}
StringField::~StringField() {}

Field* StringField::createNewInstance(const char* aName) const {
    string myName = aName==0 ? this->name : aName;
    return (Field*) new StringField(&myName);
}

void StringField::setValue(const string* valore){
    this->stringValue= new string(valore->c_str());
    if ( ((unsigned long)valore!=0) && valore->size()>0 )
        nullo = false;
}
const string* StringField::getValue() const{ return this->stringValue; }

string StringField::toString() const {
    return string( this->getValue()->c_str() );
}
