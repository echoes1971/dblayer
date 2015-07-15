/***************************************************************************
**	booleanfield.cpp  v0.1.0 - 2012.03.19
**	-------------------------------------
**
**	Author:		Roberto Rocco Angeloni.
**	E-mail:		roberto@roccoangeloni.it
**	Comment:
**	To Do:
**	Future:
**	History:
**		v0.1.0 - 2006.05.26
**
** @copyright &copy; 2011-2015 by Roberto Rocco Angeloni <roberto@roccoangeloni.it>
** @license http://opensource.org/licenses/lgpl-3.0.html GNU Lesser General Public License, version 3.0 (LGPLv3)
** @version $Id: booleanfield.cpp $
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


#include "booleanfield.h"
using namespace SchemaNS;

BooleanField::BooleanField() : Field() {
  this->type = Field::BOOLEAN;
  this->nullo = true;
}
BooleanField::BooleanField(const string& nome) : Field(nome) {
  this->type = Field::BOOLEAN;
}
BooleanField::BooleanField(const string& nome, bool valore) : Field(nome) {
  this->type = Field::BOOLEAN;
  this->setBooleanValue(valore);
}
BooleanField::~BooleanField() {}

Field* BooleanField::createNewInstance(const char* aName) const {
    string myName = aName==0 ? this->name : aName;
    Field* ret=0;
    ret = (Field*) new BooleanField(myName);
    return ret;
}

string BooleanField::toString() const { return this->boolValue ? "true" : "false"; }
