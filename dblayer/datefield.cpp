/***************************************************************************
**                        datefield.cpp  -  description
**                        -------------------
**  begin                : lun dic 2 2002
**  email                : roberto@roccoangeloni.net
**
** @copyright &copy; 2011 by Roberto Rocco Angeloni <roberto@roccoangeloni.it>
** @license http://opensource.org/licenses/lgpl-3.0.html GNU Lesser General Public License, version 3.0 (LGPLv3)
** @version $Id: datefield.cpp $
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

#include "datefield.h"

using namespace DBLayer;

DateField::DateField(const string* nome, const string* valore) : DBField(nome) {
	type = DBField::DATE;
	this->setValue( valore );
	this->setNull(false);
}
DateField::DateField(const string* nome, long seconds) : DBField(nome) {
	type = DBField::DATE;
	this->setValue(seconds);
	this->setNull(false);
}
DateField::DateField(const string* nome,
				long year, long month, long day,
                long hour, long minute, long seconds, long millis) : DBField(nome) {
	type = DBField::DATE;
	this->year    = year;
	this->month   = month;
	this->day     = day;
	this->hour    = hour;
	this->minute  = minute;
	this->seconds = seconds;
	this->millis  = millis;
	this->setNull(false);
}
DateField::~DateField() {
}

Field* DateField::createNewInstance(const char* aName) {
    Field* ret=0;
    string myName=string();
    if( aName==0 ) {
        myName.append( this->getName() );
    } else {
        myName.append( aName );
    }
    ret = new DateField(&myName,0L);
    return ret;
}

// YYYY-MM-DD HH:MM:SS mmm
void DateField::setValue(const string* valore) {
	// TODO: controllare che la stringa sia nel formato YYYY-MM-DD hh:mm:ss mmm"

	this->year = 0;	this->month = 0;	this->day = 0;
	this->hour = 0;	this->minute = 0;	this->seconds = 0;
	this->millis = 0;

	if ( valore->size() >= 10 ) {
		this->year  = atoi( valore->substr(0,4).c_str() );
		this->month = atoi( valore->substr(5,2).c_str() );
		this->day   = atoi( valore->substr(8,2).c_str() );
	}
	if ( valore->size() >=19 ) {
		this->hour    = atoi( valore->substr(11,2).c_str() );
		this->minute  = atoi( valore->substr(14,2).c_str() );
		this->seconds = atoi( valore->substr(17,2).c_str() );
	}
	if ( valore->size() >=23 ) {
		this->millis = atoi( valore->substr(20,3).c_str() );
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

bool DateField::isValidDate(long y, long m, long d) {
	return m>=1 && m<13 && d>=1 && d<=this->getDaysFor(y,m);
}

bool DateField::isValidHour(long h, long m, long s, long millis) {
	return h>=0 && h<=23 && m>=0 && m<=59 && s>=0 && s<=59
			&& millis>=0 && millis<=999;
}

long DateField::getDaysFor(long y, long m) {
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
long DateField::getDaysFor(long y) {
    return (this->getDaysFor(y,2)==29) ? 366 : 365;
}

string DateField::toString() {
    char tmp[30];
#if defined( WIN32 ) && ! defined( USING_GCC_ON_WIN32 )
    sprintf_s(
#else
    sprintf(
#endif
        tmp,
        "%04d-%02d-%02d %02d:%02d:%02d",
        (int) this->year,(int) this->month,(int) this->day,
        (int) this->hour,(int) this->minute,(int) this->seconds
    );
    return (char*)&tmp;
}

long DateField::to_seconds() {
	const static long GIORNO = 24L * 60L * 60L;
	long ret = 0;
	// Time
	ret += this->seconds;
	ret += this->minute * 60;
	ret += this->hour * 60 * 60;
	// Date
	long giorni = this->to_days();
	ret += (giorni*GIORNO);
	return ret;
}

long DateField::to_days() {
	long ret = 0;
	// Date
	ret += (this->day-1);
	for(int m=1; m<this->month; m++) {
		ret += this->getDaysFor(this->year, m);
	}
	for(int y=1970; y<this->year; y++) {
		ret += this->getDaysFor(y);
	}
	return ret;
}
