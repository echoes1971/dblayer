/***************************************************************************
**                        integerfield.cpp  -  description
**                        -------------------
**  begin                : lun dic 2 2002
**  email                : roberto@roccoangeloni.net
**
** @copyright &copy; 2011 by Roberto Rocco Angeloni <roberto@roccoangeloni.it>
** @license http://opensource.org/licenses/lgpl-3.0.html GNU Lesser General Public License, version 3.0 (LGPLv3)
** @version $Id: integerfield.cpp $
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

#include "integerfield.h"
using namespace DBLayer;

IntegerField::IntegerField(const string* nome) : DBField(nome) {
  type = DBField::INTEGER;
}
IntegerField::IntegerField(const string* nome, long valore) : DBField(nome) {
  type = DBField::INTEGER;
  this->setValue(valore);
}
//IntegerField::IntegerField(bool isPK, const string* nome, long valore) : DBField(isPK, nome) {
//  type = DBField::INTEGER;
//  this->setPK(isPK);
//  this->setValue(valore);
//}
IntegerField::~IntegerField(){
//	cout << "IntegerField::~IntegerField: " << this->name << " deleted." << endl;
}

long IntegerField::getValue(){
  return longValue;
}

void IntegerField::setValue(long valore){
  longValue=valore;
  if (valore!=0)
    nullo = false;
}


string IntegerField::toString() {
	char tmp[50];
	sprintf(
		tmp,
		"%ld",this->longValue
	);
	return string( (char*)&tmp );
}

Field* IntegerField::createNewInstance(const char* aName) {
    Field* ret=0;
    string myName=string();
    if( aName==0 ) {
        myName.append( this->getName() );
    } else {
        myName.append( aName );
    }
    ret = (Field*) new IntegerField(&myName);
    return ret;
}
