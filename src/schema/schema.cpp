/***************************************************************************
**	schema.cpp  v0.1.0 - 2012.03.19
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
** @version $Id: schema.cpp $
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

#include "booleanfield.h"
#include "datefield.h"
#include "floatfield.h"
#include "integerfield.h"
#include "stringfield.h"
#include "schema.h"
using namespace SchemaNS;

int Schema::schemiCreati = 0;
int Schema::schemiDistrutti = 0;

Schema::Schema() {
    this->name.clear();
    schemiCreati++;
}
Schema::Schema(const string* nome) {
    this->name.clear();
    this->name.append( nome->c_str() );

    schemiCreati++;
}
Schema::~Schema() {
    for(unsigned int i=0; i<this->fields.size(); i++) {
        delete this->fields.at(i);
    }
    schemiDistrutti++;
}

string Schema::getName() const { return this->name; }

string Schema::toString_nodes(string prefix) const {
    string ret=prefix;
    ret.append("<").append(this->getName()).append(">");
    // Fields
#ifdef WIN32
    size_t fieldsSize = fields.size();
#else
#ifdef __i386__
    unsigned int fieldsSize = fields.size();
#else
    unsigned long fieldsSize = fields.size();
#endif
#endif
    for(unsigned int i=0; i<fieldsSize; i++) {
        ret.append( prefix).append(" <").append(fields[i]->getName()).append(" ");
        switch(fields[i]->getType()) {
          case Field::STRING:
            ret.append("type=\"string\">").append( *(fields[i]->getStringValue()) );
            break;
          case Field::INTEGER:
            ret.append("type=\"integer\">").append( integer2string( fields[i]->getIntegerValue() ) );
            break;
          case Field::FLOAT:
            ret.append("type=\"float\">").append( fields[i]->toString() );
            break;
          case Field::BOOLEAN:
            ret.append("type=\"boolean\">").append( fields[i]->toString() );
            break;
          case Field::DATE:
            ret.append("type=\"date\">").append( fields[i]->toString() );
            break;
          default:
            ret.append("type=\"undefined\">");
            break;
        }
        ret.append("</").append(fields[i]->getName()).append(">");
    }
    ret.append(prefix).append("</").append(this->getName()).append(">");
    return ret;
}
string Schema::toString(string prefix, bool valuesAsAttributes) const {
    if ( !valuesAsAttributes ) {
        return toString_nodes(prefix);
    }
    string ret(prefix);
    ret.append("<").append( this->getName() ).append( " " );
    // Fields
#ifdef WIN32
    size_t fieldsSize = fields.size();
    for(size_t i=0; i<fieldsSize; i++) {
#else
    
#ifdef __i386__
    unsigned int fieldsSize = fields.size();
    for(unsigned int i=0; i<fieldsSize; i++) {
#else
    unsigned long fieldsSize = fields.size();
    for(unsigned long i=0; i<fieldsSize; i++) {
#endif

#endif
        ret.append( fields[i]->getName().c_str() ).append( "=\'" );
        switch(fields[i]->getType()) {
        case Field::STRING:
            ret.append( fields[i]->getStringValue()->c_str() );
            break;
        case Field::INTEGER:
            ret.append( integer2string( fields[i]->getIntegerValue() ).c_str() );
            break;
        case Field::FLOAT:
            ret.append( fields[i]->toString().c_str() );
            break;
        case Field::BOOLEAN:
            ret.append( fields[i]->toString().c_str() );
            break;
        case Field::DATE:
            ret.append( fields[i]->toString().c_str() );
            break;
        }
        ret.append( "\' " );
    }
    return ret.append( "/>" );
}

Schema* Schema::createNewInstance(const char* aName) const {
    string myName = aName==0 ? this->name : aName;
    return new Schema(&myName);
}
Schema* Schema::clone(Schema* newSchema) const {
    Schema* ret = newSchema==0 ? this->createNewInstance() : newSchema;
    int fieldSize = this->getFieldSize();
    for(int i=0; i<fieldSize; i++) {
        Field* myfield = this->getField(i);
        ret->addField( myfield->clone() );
    }
    return ret;
}

bool Schema::equals(Schema* right) const {
    bool ret=true;
    ret = ret && (this->getName() == right->getName());
    ret = ret && (this->getFieldSize() == right->getFieldSize());
    StringVector nomi = this->getNames();
#ifdef WIN32
    size_t nomiSize = nomi.size();
    for(size_t i=0; i<nomiSize; i++) {
#else

#ifdef __i386__
    unsigned int nomiSize = nomi.size();
    for(unsigned int i=0; ret && i<nomiSize; i++) {
#else
    unsigned long nomiSize = nomi.size();
    for(unsigned long i=0; ret && i<nomiSize; i++) {
#endif

#endif
        Field* otherfield = right->getField(nomi[i]);
        if(otherfield==0) {
            ret=false;
            continue;
        }
        Field* myfield = this->getField(nomi[i]);
        if(myfield==0) {
            ret=false;
            continue;
        }
        ret = ret && ( (*myfield) == (*otherfield) );
    }
    return ret;
}

DECLSPECIFIER bool SchemaNS::operator==(const Schema& left, const Schema& right) {
    Schema* left_pointer = (Schema*) &left;
    Schema* right_pointer = (Schema*) &right;
    return left_pointer->equals( right_pointer );
}

Field* Schema::createNewField(const string& fieldName, bool valore) const { return new BooleanField(fieldName, valore); }
Field* Schema::createNewField(const string& fieldName, float valore) const { return new FloatField(fieldName, valore); }
Field* Schema::createNewField(const string& fieldName, long valore) const { return new IntegerField(fieldName, valore); }
Field* Schema::createNewField(const string& fieldName, const string& valore) const { return new StringField(fieldName, valore); }
Field* Schema::createNewDateField(const string& fieldName, const string& valore) const { return new DateField(fieldName, valore); }
void Schema::addField( Field* field ) { this->fields.push_back( field ); }
Field* Schema::getField(const string& field) const {
    Field* ret = (Field*)0;
    int indice = this->getFieldIndex(field);
    if( indice>=0 )
        ret = this->getField( indice );
    return ret;
}
Field* Schema::getField(string &field) const {
    Field* ret = (Field*)0;
    int indice = this->getFieldIndex(field);
    if( indice>=0 )
        ret = this->getField( indice );
    return ret;
}
Field* Schema::getField(int i) const { return i>=0 && i<((int)this->fields.size()) ? this->fields[i] : 0; }
int Schema::getFieldIndex(const string& field) const {
    int ret = -1;
    const char* fieldCstr = field.c_str();
#ifdef WIN32
    size_t fieldsSize = fields.size();
    for(size_t i=0; i<fieldsSize && ret<0 ; i++) {
#else

#ifdef __i386__
    unsigned int fieldsSize = fields.size();
    for(unsigned int i=0; i<fieldsSize && ret<0 ; i++) {
#else
    unsigned long fieldsSize = fields.size();
    for(unsigned long i=0; i<fieldsSize && ret<0 ; i++) {
#endif

#endif
        if ( fields[i]->getName().compare( fieldCstr ) == 0 ) {
            ret = (int)i;
        }
    }
    return ret;
}
int Schema::getFieldSize() const { return (int) this->fields.size(); }

StringVector Schema::getNames() const {
    StringVector ret;
#ifdef WIN32
    size_t fieldsSize = fields.size();
    for(size_t i=0; i<fieldsSize; i++) {
#else

#ifdef __i386
    unsigned int fieldsSize = fields.size();
    for(unsigned int i=0; i<fieldsSize; i++) {
#else
    unsigned long fieldsSize = fields.size();
    for(unsigned long i=0; i<fieldsSize; i++) {
#endif

#endif
        ret.push_back( fields[i]->getName() );
    }
    return ret;
}
FieldMap Schema::getValuesDictionary() const {
    FieldMap ret;
#ifdef WIN32
    size_t fieldsSize = fields.size();
    for(size_t i=0; i<fieldsSize; i++) {
#else

#ifdef __i386
    unsigned int fieldsSize = fields.size();
    for(unsigned int i=0; i<fieldsSize; i++) {
#else
    unsigned long fieldsSize = fields.size();
    for(unsigned long i=0; i<fieldsSize; i++) {
#endif

#endif
        if( !this->fields[i]->isNull() )
            ret[ this->fields[i]->getName() ] = this->fields[i];
    }
    return ret;
}

void Schema::setDateValue(const string& fieldName, const string& valore) {
    Field* field = this->getField(fieldName);
    if(field==0) {
        field = this->createNewDateField(fieldName, valore);
        this->addField(field);
    } else {
        ((DateField*)field)->setValue(valore);
    }
}
void Schema::setValue(const string& fieldName, const string& valore) {
    Field* field = this->getField(fieldName);
    if(field==0) {
        field = this->createNewField(fieldName, valore);
        this->addField(field);
    } else {
        field->setStringValue(valore);
    }
}
void Schema::setValue(const string& fieldName, bool valore) {
    Field* field = this->getField(fieldName);
    if(field==0) {
        field =  this->createNewField(fieldName, valore);
        this->addField(field);
    } else if(!field->isBoolean()) {
        int indice = this->getFieldIndex(fieldName);
        delete field;
        field=0;
        this->fields[indice] = new BooleanField(fieldName, valore);
    } else {
        field->setBooleanValue(valore);
    }
}
void Schema::setValue(const string& fieldName, float valore) {
	Field* field = this->getField(fieldName);
    if(field==0) {
        field =  this->createNewField(fieldName, valore);
		this->addField(field);
    } else if(!field->isFloat()) {
        int indice = this->getFieldIndex(fieldName);
        delete field;
        field=0;
        this->fields[indice] = new FloatField(fieldName, valore);
    } else {
		field->setFloatValue(valore);
	}
}
void Schema::setValue(const string& fieldName, long valore) {
	Field* field = this->getField(fieldName);
    if(field==0) {
        field =  this->createNewField(fieldName, valore);
		this->addField(field);
    } else if(!field->isInteger()) {
        int indice = this->getFieldIndex(fieldName);
        delete field;
        field=0;
        this->fields[indice] = new IntegerField(fieldName, valore);
	} else {
		field->setIntegerValue(valore);
	}
}

string Schema::getValue(const string& fieldName) const {
    Field* field = this->getField(fieldName);
    return field==0 ? "" : field->toString();
}
string Schema::getDateValue(const string& fieldName) const {
    Field* field = this->getField(fieldName);
    return field==0 ? "" : field->toString();
}

bool Schema::isNull(const string &fieldName) const {
	Field* field = this->getField(fieldName);
	return field==0 ? true : field->isNull();
}
void Schema::setNull(const string& fieldName) {
	Field* field = this->getField(fieldName);
	if( field!=0 ) field->setNull();
}
void Schema::setNull(const string& fieldName, bool valore) {
	Field* field = this->getField(fieldName);
	if( field!=0 ) field->setNull(valore);
}


//********************** UTILITY

string SchemaNS::integer2string(long longValue) {
    int i = 50-2;
    char tmp[50];  tmp[50-2] = '0';  tmp[50-1] = '\0';
    if ( longValue<0 ) {
        return string("-").append( SchemaNS::integer2string( -1 * longValue ) );
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

string SchemaNS::float2string(float f) {
    char tmp[50];
#if defined( WIN32 ) && ! defined( USING_GCC_ON_WIN32 )
    sprintf_s(tmp,"%f",f);
#else
    sprintf(tmp,"%f",f);
#endif
    return string( (char*)&tmp );
}


int SchemaNS::getSchemiCreati() { return Schema::getSchemiCreati(); }
int SchemaNS::getSchemiDistrutti() { return Schema::getSchemiDistrutti(); }
int Schema::getSchemiCreati() { return Schema::schemiCreati; }
int Schema::getSchemiDistrutti() { return Schema::schemiDistrutti; }
