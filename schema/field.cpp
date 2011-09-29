/***************************************************************************
**	field.cpp  v0.1.0 - 2006.05.26
**	------------------------------
**
**	Author:		Roberto Rocco Angeloni.
**	email:		roberto@roccoangeloni.it
**	Copyright:	(c) 2006 by Roberto Rocco Angeloni.
**	Comment:
**	To Do:		- ...
**	Future:
**	History:
**		v0.1.0 - 2006.05.26 Versione iniziale
**
** @copyright &copy; 2011 by Roberto Rocco Angeloni <roberto@roccoangeloni.it>
** @license http://opensource.org/licenses/lgpl-3.0.html GNU Lesser General Public License, version 3.0 (LGPLv3)
** @version $Id: field.cpp $
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

#include <cstring>
#include <stdlib.h>
#include <cstdio>

#include "field.h"
using namespace SchemaNS;

int Field::fieldCreati = 0;
int Field::fieldDistrutti = 0;

Field::Field() {
  this->nullo = true;
  this->stringValue=0;
  fieldCreati++;
}
Field::Field(const string* nome) {
    this->name=*nome;
    this->nullo = true;
    this->stringValue=0;
    fieldCreati++;
}
Field::~Field() {
    if( this->stringValue!=0 ) delete this->stringValue;
    fieldDistrutti++;
}

Field* Field::createNewInstance(const char* aName) {
    Field* ret=0;
    string myName = aName==0 ? this->name : aName ;
    ret = new Field(&myName);
    ret->type = this->type;
    return ret;
}
Field* Field::clone() {
    Field* ret = this->createNewInstance();
    switch( this->getType() ) {
    case Field::BOOLEAN:
        ret->setBooleanValue( this->getBooleanValue() );
        break;
    case Field::INTEGER:
        ret->setIntegerValue( this->getIntegerValue() );
        break;
    case Field::STRING:
        ret->setStringValue( this->getStringValue() );
        break;
    case Field::FLOAT:
        ret->setFloatValue( this->getFloatValue() );
        break;
    }
    return ret;
}

bool Field::equals(Field* field) {
    bool ret=true;
    ret = ret && (field->getName() == this->getName());
    ret = ret && (field->getType() == this->getType());
    switch( this->getType() ) {
    case Field::BOOLEAN:
        ret = ret && (field->getBooleanValue() == this->getBooleanValue());
        break;
    case Field::INTEGER:
        ret = ret && (field->getIntegerValue() == this->getIntegerValue());
        break;
    case Field::STRING:
        ret = ret && (strcmp(field->getStringValue()->c_str(),this->getStringValue()->c_str())==0);
        break;
    case Field::FLOAT:
        ret = ret && (field->getFloatValue() == this->getFloatValue());
        break;
    }
    return ret;
}
string Field::getTypeName() {
    switch(this->type) {
      case Field::ANY:
        return "ANY";
      case Field::BOOLEAN:
        return "BOOLEAN";
      case Field::DATE:
        return "DATE";
      case Field::FLOAT:
        return "FLOAT";
      case Field::INTEGER:
        return "INTEGER";
      case Field::STRING:
        return "STRING";
    }
    return "UNKNOWN";
}
bool Field::isBoolean() { return type==Field::BOOLEAN; }
bool Field::isInteger() { return type==Field::INTEGER; }
bool Field::isString() { return type==Field::STRING; }
bool Field::isDate() { return type==Field::DATE; }
bool Field::isFloat() { return type==Field::FLOAT; }
bool Field::isDouble() { return type==Field::FLOAT; }

long Field::getIntegerValue() { return this->longValue; }
float Field::getFloatValue() { return this->floatValue; }
const string* Field::getStringValue() { return this->stringValue; }
bool Field::getBooleanValue() { return this->boolValue; }

void Field::setStringValue(const string* valore) {
    if(this->stringValue!=0) delete this->stringValue;
    this->stringValue = new string(valore->c_str());
    nullo = false;
}
void Field::setStringValue(string valore) {
    if(this->stringValue!=0) delete this->stringValue;
    this->stringValue = new string( valore.c_str() );
    nullo = false;
}
void Field::setFloatValue(float valore) {
  floatValue = valore;
  nullo = false;
}
void Field::setIntegerValue(long valore) {
  longValue = valore;
  nullo = false;
}
void Field::setBooleanValue(bool valore) {
  boolValue = valore;
  nullo = false;
}
// YYYY-MM-DD HH:MM:SS
void Field::setDateValue(const string* valore) {
    // TODO: controllare che la stringa sia nel formato YYYY-MM-DD hh:mm:ss mmm"

    int year = 0; int month = 0;  int day = 0;
    int hour = 0; int minute = 0; int seconds = 0;

    if ( valore->size() >= 10 ) {
        year  = atoi( valore->substr(0,4).c_str() );
        month = atoi( valore->substr(5,2).c_str() );
        day   = atoi( valore->substr(8,2).c_str() );
    }
    if ( valore->size() >= 19 ) {
        hour    = atoi( valore->substr(11,2).c_str() );
        minute  = atoi( valore->substr(14,2).c_str() );
        seconds = atoi( valore->substr(17,2).c_str() );
    }

    this->longValue = Field::to_seconds(hour,minute,seconds,
                          Field::to_days(year,month,day)
                          );

    this->setNull(false);
}

void Field::setValue(string valore) {
	if(this->stringValue!=0) delete this->stringValue;
	this->stringValue = new string( valore.c_str() );
    if(this->nullo) {
        if(this->type==Field::ANY)
            this->type=Field::STRING;
        this->nullo = false;
    }
}
void Field::setValue(float valore) {
  floatValue = valore;
  if(this->nullo) {
      if(this->type==Field::ANY)
          this->type=Field::FLOAT;
      this->nullo = false;
  }
}
void Field::setValue(long valore) {
  longValue = valore;
  if(this->nullo) {
      if(this->type==Field::ANY)
          this->type=Field::INTEGER;
      this->nullo = false;
  }
}
void Field::setValue(bool valore) {
  boolValue = valore;
  if(this->nullo) {
      if(this->type==Field::ANY)
          this->type=Field::BOOLEAN;
      this->nullo = false;
  }
}

void Field::setValue(void* value) {
    if(value==0)
        return;
    switch(type) {
      case BOOLEAN:
        this->setValue( (bool)value );
        break;
      case INTEGER:
        this->setValue( (long)value );
        break;
      case FLOAT:
        this->setValue( *static_cast<float*>(value) );
        break;
      case DATE:
        this->setDateValue( (string*)value );
        break;
      default:
        this->setValue( string( ((string*)value)->c_str() ) );
        break;
    }
}
void* Field::getValue() {
    if(this->isNull()) return 0;
    switch(type) {
      case BOOLEAN:
        return (void*)this->getBooleanValue();
        break;
      case INTEGER:
        return (void*)this->getIntegerValue();
        break;
      case FLOAT:
        return (void*)this->getStringValue();
        // @TODO return static_cast<void*>(this->getFloatValue());
        break;
      case DATE:
        return (void*)this->getStringValue();
        break;
      default:
        return (void*)this->getStringValue();
        break;
    }
}
string Field::getValueAsString() {
    if(this->isNull()) return "<NULL>";
    switch(type) {
      case BOOLEAN:
        return this->getBooleanValue()?"true":"false";
        break;
      case INTEGER:
        return SchemaNS::integer2string(this->getIntegerValue());
        break;
      case FLOAT:
        return SchemaNS::float2string(this->getFloatValue());
        break;
        /*
      case DATE:
        return this->getStringValue();
        break;
        */
      default:
        string ret = this->getStringValue()==0 ?
                     "" : string(this->getStringValue()->c_str());
        return ret;
        break;
    }
    return "";
}

string Field::toString() { return ""; }

bool Field::isNull() { return nullo; }
void Field::setNull() { nullo=true; }
void Field::setNull(bool valore) { nullo = valore; }

long Field::getDaysFor(long y, long m) {
    long ret = -1;
    switch (m) {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            ret = 31;
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            ret = 30;
            break;
        case 2:
            if (y % 4 == 0)
                ret = 29;
            else
                ret = 28;
            if ((y % 100 == 0) && (y % 400 != 0))
                ret = 28;
    }
    return ret;
}
long Field::getDaysFor(long y) { return (Field::getDaysFor(y,2)==29) ? 366 : 365; }
long Field::to_days(long year, long month, long day) {
    long ret = 0;
    // Date
    ret += (day-1);
    for(int m=1; m<month; m++) {
        ret += Field::getDaysFor(year, m);
    }
    for(int y=1970; y<year; y++) {
        ret += Field::getDaysFor(y);
    }
    return ret;
}
long Field::to_seconds(long hour, long minutes, long seconds, long days) {
    const static long GIORNO = 24L * 60L * 60L;
    long ret = 0;
    // Time
    ret += seconds;
    ret += minutes * 60;
    ret += hour * 60 * 60;
    // Date
    ret += (days*GIORNO);
    return ret;
}


bool SchemaNS::operator==(const Field& left, const Field& right) {
    Field* left_pointer = (Field*) &left;
    Field* right_pointer = (Field*) &right;
    return left_pointer->equals( right_pointer );
}
int SchemaNS::getFieldCreati() { return Field::getFieldCreati(); }
int SchemaNS::getFieldDistrutti() { return Field::getFieldDistrutti(); }
int Field::getFieldCreati() { return Field::fieldCreati; }
int Field::getFieldDistrutti() { return Field::fieldDistrutti; }
