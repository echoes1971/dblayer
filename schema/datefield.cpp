/***************************************************************************
**	datefield.cpp  v0.1.0 - 2006.05.26
**	----------------------------------
**
**	Author:		Roberto Rocco Angeloni.
**	email:		roberto@roccoangeloni.it
**	Copyright:	(c) 2006 by Roberto Rocco Angeloni.
**	Comment:	Implementazione di connection e resultset per ODBC
**	To Do:		- ...
**	Future:
**	History:
**		v0.1.0 - 2006.05.26 Versione iniziale
**
** @copyright &copy; 2011 by Roberto Rocco Angeloni <roberto@roccoangeloni.it>
** @license http://opensource.org/licenses/lgpl-3.0.html GNU Lesser General Public License, version 3.0 (LGPLv3)
** @version $Id: datefield.cpp $
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

#include <stdlib.h>
#include <cstdio>

#include "datefield.h"

using namespace SchemaNS;

DateField::DateField(const string* nome, const string* valore) : Field(nome){
    type = Field::DATE;
    this->setValue( valore );
    this->setNull(false);
}
DateField::DateField(const string* nome, long seconds) : Field(nome){
    type = Field::DATE;
    this->setValue(seconds);
    this->setNull(false);
}
DateField::DateField(const string* nome,
                     long year, long month, long day,
                     long hour, long minute, long seconds, long millis) : Field(nome) {
    type = Field::DATE;
    this->year    = year;
    this->month   = month;
    this->day     = day;
    this->hour    = hour;
    this->minute  = minute;
    this->seconds = seconds;
    this->millis  = millis;
    this->setNull(false);
}
DateField::~DateField(){}

Field* DateField::createNewInstance(const char* aName) {
    string myName = aName==0 ? this->name : aName;
    return new DateField(&myName,0L);
}

Field* DateField::clone() {
    DateField* ret = (DateField*) this->createNewInstance();
    ret->year=this->year;
    ret->month=this->month;
    ret->day=this->day;
    ret->hour=this->hour;
    ret->minute=this->minute;
    ret->seconds=this->seconds;
    ret->millis=this->millis;
    return ret;
}

bool DateField::equals(Field* field) {
    bool ret=true;
    ret = ret && (field->getName() == this->getName());
    ret = ret && (field->getType() == this->getType());
    ret = ret && (((DateField*)field)->year == this->year);
    ret = ret && (((DateField*)field)->month == this->month);
    ret = ret && (((DateField*)field)->day == this->day);
    ret = ret && (((DateField*)field)->hour == this->hour);
    ret = ret && (((DateField*)field)->minute == this->minute);
    ret = ret && (((DateField*)field)->seconds == this->seconds);
    ret = ret && (((DateField*)field)->millis == this->millis);
    return ret;
}



// YYYY-MM-DD HH:MM:SS mmm
void DateField::setValue(const string* valore) {
    // TODO: controllare che la stringa sia nel formato YYYY-MM-DD hh:mm:ss mmm"

    const char* tmpValore = valore->c_str();
#ifdef WIN32
    size_t tmpSize = valore->size();
#else
 #ifdef __i386__
    unsigned int tmpSize = valore->size();
 #else
    unsigned long tmpSize = valore->size();
 #endif
#endif

    this->year = 0;	this->month = 0;	this->day = 0;
    this->hour = 0;	this->minute = 0;	this->seconds = 0;
    this->millis = 0;

    if ( tmpSize >= 10 ) {
        char _year[]={'\0','\0','\0','\0','\0'};
        char _month[]={'\0','\0','\0'};
        char _day[]={'\0','\0','\0'};
        _year[0]  = tmpValore[0]; _year[1] = tmpValore[1]; _year[2] = tmpValore[2]; _year[3] = tmpValore[3];
        _month[0] = tmpValore[5]; _month[1] = tmpValore[6];
        _day[0]   = tmpValore[8]; _day[1] = tmpValore[9];
        this->year  = atoi( (const char*) &_year );
        this->month = atoi( (const char*) &_month );
        this->day   = atoi( (const char*) &_day );
//        this->year  = atoi( valore->substr(0,4).c_str() );
//        this->month = atoi( valore->substr(5,2).c_str() );
//        this->day   = atoi( valore->substr(8,2).c_str() );
    }
    if ( tmpSize >= 19 ) {
        char _hour[]={'\0','\0','\0'};
        char _minute[]={'\0','\0','\0'};
        char _seconds[]={'\0','\0','\0'};
        _hour[0]   = tmpValore[11]; _hour[1] = tmpValore[12];
        _minute[0] = tmpValore[14]; _minute[1] = tmpValore[15];
        _seconds[0] = tmpValore[17]; _seconds[1] = tmpValore[18];
        this->hour = atoi( (const char*) &_hour );
        this->minute = atoi( (const char*) &_minute );
        this->seconds = atoi( (const char*) &_seconds );
//        this->hour    = atoi( valore->substr(11,2).c_str() );
//        this->minute  = atoi( valore->substr(14,2).c_str() );
//        this->seconds = atoi( valore->substr(17,2).c_str() );
    }
    if ( tmpSize >= 23 ) {
        char _millis[]={'\0','\0','\0','\0','\0'};
        _millis[0]  = tmpValore[20]; _millis[1] = tmpValore[21]; _millis[2] = tmpValore[22]; _millis[3] = tmpValore[23];
        this->millis = atoi( (const char*) &_millis );
//        this->millis = atoi( valore->substr(20,3).c_str() );
    }

    this->setNull(false);
}
void DateField::setValue(long seconds) {
    this->year = 0;	this->month = 0;	this->day = 0;
    this->hour = 0;	this->minute = 0;	this->seconds = 0;
    this->millis = 0;

    this->seconds = seconds%60;
    long minuti = (long)seconds/60;

    this->minute=minuti%60;
    long ore = (long)minuti/60;

    this->hour=ore%24;
    long giorni = (long)ore/24 + 1;

    // Anno
    int y=1970;
    long mygiorni = this->getDaysFor(y);
    while( (giorni - mygiorni) > 0 ) {
        y++;
        giorni -= mygiorni;
        mygiorni = this->getDaysFor(y);
    }
    this->year=y;

    // Mese
    int m=1;
    mygiorni = this->getDaysFor(this->year,m);
    while( (giorni - mygiorni) > 0 && m<13 ) {
        m++;
        giorni -= mygiorni;
        mygiorni = this->getDaysFor(this->year,m);
    }
    this->month=m;

    this->day = giorni;

    this->setNull(false);
}

bool DateField::isValidDate(long y, long m, long d) { return m>=1 && m<13 && d>=1 && d<=this->getDaysFor(y,m); }
bool DateField::isValidHour(long h, long m, long s, long millis) { return h>=0 && h<=23 && m>=0 && m<=59 && s>=0 && s<=59 && millis>=0 && millis<=999; }

string DateField::toString() {
    char tmp[50];
#if defined( WIN32 ) && ! defined( USING_GCC_ON_WIN32 )
    sprintf_s(
                tmp,
                "%04d-%02d-%02d %02d:%02d:%02d",
                (int) this->year,(int) this->month,(int) this->day,
                (int) this->hour,(int) this->minute,(int) this->seconds
                );
#else
    sprintf(
                tmp,
                "%04d-%02d-%02d %02d:%02d:%02d",
                (int) this->year,(int) this->month,(int) this->day,
                (int) this->hour,(int) this->minute,(int) this->seconds
                );
#endif
    return string( (char*)&tmp );
}

long DateField::to_seconds() { return Field::to_seconds(this->hour,this->minute,this->seconds,Field::to_days(this->year,this->month,this->day)); }
long DateField::to_days() { return Field::to_days(this->year,this->month,this->day); }
