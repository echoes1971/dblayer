/***************************************************************************
**	floatfield.h  v0.1.0 - 2012.03.19
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
** @copyright &copy; 2011-2015 by Roberto Rocco Angeloni <roberto@roccoangeloni.it>
** @license http://opensource.org/licenses/lgpl-3.0.html GNU Lesser General Public License, version 3.0 (LGPLv3)
** @version $Id: floatfield.cpp $
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

#include <cstdio>

#include "floatfield.h"
using namespace SchemaNS;

FloatField::FloatField(const string& nome) : Field(nome) {
    this->type = Field::FLOAT;
}
FloatField::FloatField(const string& nome, float valore) : Field(nome) {
    this->type = Field::FLOAT;
    this->setValue(valore);
}
FloatField::~FloatField() {}

Field* FloatField::createNewInstance(const char* aName) const {
    string myName = aName==0 ? this->name : aName;
    return new FloatField(myName);
}

float FloatField::getValue() const { return floatValue; }
void FloatField::setValue(float valore){
    floatValue = valore;
    nullo = false;
    this->stringRepr = float2string(this->floatValue);
}

string FloatField::toString() const  { return this->stringRepr; }
