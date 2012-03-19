/***************************************************************************
**	xmlrpcconnection.cpp  v0.1.0 - 2009.05.22
**	-----------------------------------------
**
**	Author:		Roberto Rocco Angeloni.
**	email:		roberto@roccoangeloni.it
**	Comment:	Implementazione di connection e resultset per Xmlrpc
**	To Do:		- 
**	Future:
**	History:
**		v0.1.0 - 2009.05.22 Iniziato lo sviluppo
**
** @copyright &copy; 2011 by Roberto Rocco Angeloni <roberto@roccoangeloni.it>
** @license http://opensource.org/licenses/lgpl-3.0.html GNU Lesser General Public License, version 3.0 (LGPLv3)
** @version $Id: xmlrpcconnection.cpp $
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
#ifdef WIN32
#include "stdafx.h"
using namespace System;
#else
#include <config.h>
#endif

#include "xmlrpcconnection.h"
using namespace DBLayer;

#ifdef USE_LIBXMLRPC


//********************* XmlrpcConnection: inizio.

#ifdef WIN32
XmlrpcConnection::XmlrpcConnection(string s) : Connection(s) {
#else
XmlrpcConnection::XmlrpcConnection(string s) : Connection::Connection(s) {
#endif
}
XmlrpcConnection::~XmlrpcConnection() {
	this->disconnect();
}

bool XmlrpcConnection::connect() {
    xmlrpc_c::value result;
    this->connected=false;
    this->errorMessage.clear();
    try {
        this->_client.call(this->connectionString, "selectAsArray", "ss", &result, "nometabella"
                           , "show tables"
                           //, "select * from rra_users"
                           );
    } catch (std::exception const& e) {
        this->errorMessage.append( "Client threw error: " ); this->errorMessage.append( e.what() );
        return false;
    } catch (...) {
        this->errorMessage.append( "Client threw unexpected error." );
        return false;
    }

    // Voglio che result sia del tipo [ stringa, [] ], altrimenti errore
    if(result.type()!=xmlrpc_c::value::TYPE_ARRAY) {
        this->errorMessage.append( "Server returned a wrong type: " + integer2string(result.type())
                                   + " instead of " + integer2string(xmlrpc_c::value::TYPE_ARRAY) );
        return false;
    }

    xmlrpc_c::value_array myarray(result);

    if( myarray.vectorValueValue().at(0).type()!=xmlrpc_c::value::TYPE_STRING
            && myarray.vectorValueValue().at(0).type()!=xmlrpc_c::value::TYPE_BYTESTRING ) {
        this->errorMessage.append( "Server returned a wrong type at return[0]: " + integer2string(myarray.vectorValueValue().at(0).type())
                                   + " instead of " + integer2string(xmlrpc_c::value::TYPE_STRING)
                                   + " or " + integer2string(xmlrpc_c::value::TYPE_BYTESTRING) );
        return false;
    }
    if( myarray.vectorValueValue().at(1).type()!=xmlrpc_c::value::TYPE_ARRAY ) {
        this->errorMessage.append( "Server returned a wrong type at return[1]: " + integer2string(myarray.vectorValueValue().at(1).type())
                                   + " instead of " + integer2string(xmlrpc_c::value::TYPE_ARRAY) );
        return false;
    }

    this->connected=true;
    return true;
}
bool XmlrpcConnection::disconnect() { this->connected=false; return true; }
bool XmlrpcConnection::reconnect() { return this->connect(); }

ResultSet* XmlrpcConnection::exec(const string s) {
    // 2011.09.26 - XmlrpcResultSet* rs = new XmlrpcResultSet();
    xmlrpc_c::value result;
    this->errorMessage.clear();
    try {
        this->_client.call(this->connectionString, "selectAsArray", "ss", &result, "nometabella", s.c_str());
    } catch (std::exception const& e) {
        this->errorMessage.append( "Client threw error: " ); this->errorMessage.append( e.what() );
        return false;
    } catch (...) {
        this->errorMessage.append( "Client threw unexpected error." );
        return false;
    }

    // Voglio che result sia del tipo [ stringa, [] ], altrimenti errore
    if(result.type()!=xmlrpc_c::value::TYPE_ARRAY) {
        this->errorMessage.append( "Server returned a wrong type: " + integer2string(result.type())
                                   + " instead of " + integer2string(xmlrpc_c::value::TYPE_ARRAY) );
        return false;
    }

    xmlrpc_c::value_array myarray(result);

    if( myarray.vectorValueValue().at(0).type()!=xmlrpc_c::value::TYPE_STRING
            && myarray.vectorValueValue().at(0).type()!=xmlrpc_c::value::TYPE_BYTESTRING ) {
        this->errorMessage.append( "Server returned a wrong type at return[0]: " + integer2string(myarray.vectorValueValue().at(0).type())
                                   + " instead of " + integer2string(xmlrpc_c::value::TYPE_STRING)
                                   + " or " + integer2string(xmlrpc_c::value::TYPE_BYTESTRING) );
        return false;
    }
    if( myarray.vectorValueValue().at(1).type()!=xmlrpc_c::value::TYPE_ARRAY ) {
        this->errorMessage.append( "Server returned a wrong type at return[1]: " + integer2string(myarray.vectorValueValue().at(1).type())
                                   + " instead of " + integer2string(xmlrpc_c::value::TYPE_ARRAY) );
        return false;
    }

    xmlrpc_c::value_array lista(myarray.vectorValueValue().at(1));

    return XmlrpcConnection::list2resultset( &lista, new XmlrpcResultSet() );
}

string XmlrpcConnection::escapeString(string s) { return DBLayer::replaceAll(s, "\'", "\'\'"); }

int XmlrpcConnection::getColumnSize(string* relname) { return -1; }
string XmlrpcConnection::getColumnName(string* relname, int column) { return ""; }
IntegerVector XmlrpcConnection::getKeys(string* relname) {
    IntegerVector ret;
    this->errorMessage.append("XmlrpcConnection::getKeys: UNSUPPORTED; relname=" + string(relname->c_str()) );
    return ret;
}
IntegerVector XmlrpcConnection::getForeignKeys(string* relname) {
    IntegerVector ret;
    this->errorMessage.append("XmlrpcConnection::getForeignKeys: UNSUPPORTED; relname=" + string(relname->c_str()) );
    return ret;
}


ResultSet* XmlrpcConnection::login(string user, string pwd) {
    xmlrpc_c::value result;
    this->errorMessage.clear();
    try {
        this->_client.call(this->connectionString, "login", "ss", &result, user.c_str(), pwd.c_str());
    } catch (std::exception const& e) {
        this->errorMessage.append( "Client threw error: " ); this->errorMessage.append( e.what() );
        return false;
    } catch (...) {
        this->errorMessage.append( "Client threw unexpected error." );
        return false;
    }

    if(result.type()!=xmlrpc_c::value::TYPE_ARRAY) {
        this->errorMessage.append( "Server returned a wrong type: " + integer2string(result.type())
                                   + " instead of " + integer2string(xmlrpc_c::value::TYPE_ARRAY) );
        return 0;
    }
    xmlrpc_c::value_array myarray(result);

    if( myarray.vectorValueValue().at(0).type()!=xmlrpc_c::value::TYPE_STRING
            && myarray.vectorValueValue().at(0).type()!=xmlrpc_c::value::TYPE_BYTESTRING ) {
        this->errorMessage.append( "Server returned a wrong type at return[0]: " + integer2string(myarray.vectorValueValue().at(0).type())
                                   + " instead of " + integer2string(xmlrpc_c::value::TYPE_STRING)
                                   + " or " + integer2string(xmlrpc_c::value::TYPE_BYTESTRING) );
        return false;
    }
    if( myarray.vectorValueValue().at(1).type()!=xmlrpc_c::value::TYPE_ARRAY ) {
        this->errorMessage.append( "Server returned a wrong type at return[1]: " + integer2string(myarray.vectorValueValue().at(1).type())
                                   + " instead of " + integer2string(xmlrpc_c::value::TYPE_ARRAY) );
        return false;
    }

    xmlrpc_c::value_array lista(myarray.vectorValueValue().at(1));

    ResultSet* ret = XmlrpcConnection::list2resultset( &lista, new XmlrpcResultSet() );

    return ret;
}

XmlrpcResultSet* XmlrpcConnection::list2resultset(xmlrpc_c::value_array* iLista, XmlrpcResultSet* ioResultSet) {
    // Map
    std::map<std::string, xmlrpc_c::value> m;
    // Iterator
    std::map<std::string, xmlrpc_c::value>::const_iterator i;
    for(unsigned int r=0; r<iLista->size(); r++) {
        // Struct
        xmlrpc_c::value_struct v( iLista->vectorValueValue().at(r) );
        // Map
        m = (std::map<std::string, xmlrpc_c::value>) v;
        unsigned int colonna=0;
        string my_string;
        for(i=m.begin(); i!=m.end(); ++i) {
            // Metadati
            if(ioResultSet->columnName.size()<=colonna) ioResultSet->columnName.push_back( i->first );
            my_string.clear();
            switch(i->second.type()) {
            case xmlrpc_c::value::TYPE_INT:
                if(ioResultSet->columnType.size()<=colonna) ioResultSet->columnType.push_back( DBLayer::type_integer );
                ioResultSet->righe.push_back( XmlrpcResultSet::integer2string( (int) xmlrpc_c::value_int(i->second) ) );
                break;
            case xmlrpc_c::value::TYPE_BOOLEAN:
                if(ioResultSet->columnType.size()<=colonna) ioResultSet->columnType.push_back( DBLayer::type_integer );
                ioResultSet->righe.push_back( XmlrpcResultSet::integer2string( ( (bool) xmlrpc_c::value_boolean(i->second) ) ? 1 : 0 ) );
                break;
            case xmlrpc_c::value::TYPE_DOUBLE:
                if(ioResultSet->columnType.size()<=colonna) ioResultSet->columnType.push_back( DBLayer::type_double );
                ioResultSet->righe.push_back( XmlrpcResultSet::double2string( (double) ((float) xmlrpc_c::value_double(i->second)) ) );
                break;
            case xmlrpc_c::value::TYPE_STRING:
                if(ioResultSet->columnType.size()<=colonna) ioResultSet->columnType.push_back( DBLayer::type_string );
                ioResultSet->righe.push_back( xmlrpc_c::value_string(i->second) );
                break;
            case xmlrpc_c::value::TYPE_BYTESTRING:
                if(ioResultSet->columnType.size()<=colonna) ioResultSet->columnType.push_back( DBLayer::type_string );
                XmlrpcResultSet::bytestringToString( (xmlrpc_c::value*) &(i->second), &my_string );
                ioResultSet->righe.push_back( my_string );
                break;
            //	case xmlrpc_c::value::TYPE_ARRAY:
            //	case xmlrpc_c::value::TYPE_STRUCT:
            //	case xmlrpc_c::value::TYPE_DATETIME:
            //	case xmlrpc_c::value::TYPE_C_PTR:
            //	case xmlrpc_c::value::TYPE_NIL:
            //	case xmlrpc_c::value::TYPE_I8:
            //	case xmlrpc_c::value::TYPE_DEAD:
            default:
                if(ioResultSet->columnType.size()<=colonna) ioResultSet->columnType.push_back( DBLayer::type_blob );
                xmlrpc_c::value tmp_value = xmlrpc_c::value(i->second);
                XmlrpcResultSet::valueToString( &tmp_value, &my_string );
                ioResultSet->righe.push_back( my_string );
                break;
            }
            colonna++;
        }
    }

    return ioResultSet;
}
//********************* XmlrpcConnection: fine.


//********************* XmlrpcResultSet: inizio.
#ifdef WIN32
XmlrpcResultSet::XmlrpcResultSet() : ResultSet() {
#else
XmlrpcResultSet::XmlrpcResultSet() : ResultSet::ResultSet() {
#endif
}
XmlrpcResultSet::~XmlrpcResultSet() {}

int XmlrpcResultSet::getColumnSize(int i) {
    return -i;
}
string XmlrpcResultSet::toString(string prefix) {
    string ret;
    ret.append(prefix+"<XmlrpcResultSet>");
    // Columns
    int nColonne = this->getNumColumns();
    ret.append(prefix+" <Columns>" );
    for( int i=0; i<nColonne; i++) {
        ret.append(prefix+"  <Column ");
        ret.append("position=\'"+DBLayer::integer2string((long)i)+"\' ");
        ret.append("name=\'" + this->getColumnName(i)+"\' ");
        ret.append("type=\'" + this->getColumnType(i)+"\' ");
        ret.append("size=\'" + DBLayer::integer2string((long)this->getColumnSize(i))+"\' ");
        ret.append("/>");
    }
    ret.append(prefix+" </Columns>" );
    // Rows
    ret.append(prefix+" <Rows>");
    int nRighe = this->getNumRows();
    for(int r=0; r<nRighe; r++) {
        ret.append(prefix+"  <Row num=\'"+DBLayer::integer2string((long)r)+"\'>");
        for(int c=0; c<nColonne; c++) {
            string nomeColonna = this->getColumnName(c);
            if (! this->isNull(r,c) ) {
                ret.append(prefix+"   <"+nomeColonna+">");
                ret.append( this->getValue(r,c) );
                ret.append("</"+nomeColonna+">");
            } else {
                ret.append(prefix+"   <"+nomeColonna+" />");
            }
        }
        ret.append(prefix+"  </Row>");
    }
    ret.append(prefix+" </Rows>");

    ret.append(prefix+"</XmlrpcResultSet>");
    return ret;
}

std::string XmlrpcResultSet::integer2string(long longValue) {
    int i = 50-2;
    char tmp[50];  tmp[50-2] = '0';  tmp[50-1] = '\0';
    if ( longValue<0 ) {
        std::string tmpString = std::string( "-" );
        tmpString.append( integer2string( -1 * longValue ) );
        return tmpString;
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
    return std::string( (char*)&tmp[i+1] );
}
std::string XmlrpcResultSet::double2string(double longValue) {
    int i = 50-2;
    char tmp[50];  tmp[50-2] = '0';  tmp[50-1] = '\0';
    if ( longValue<0 ) {
        std::string tmpString = std::string( "-" );
        tmpString.append( double2string( -1 * longValue ) );
        return tmpString;
    }
    std::string parte_intera;
    if ( longValue>=1 ) {
        parte_intera.append( integer2string( (long) longValue ) );
    }
    double decimale = longValue - ((long)longValue);
    if( decimale>0 && decimale<1 ) {
        parte_intera.append( "." );
        long decimale_long = 0;
        int num_decimali = 5;
        while( decimale>0 && decimale<1 && decimale!=0.0 && num_decimali>0 ) {
            decimale_long = decimale_long*10 + (long)(decimale*10);
            decimale = (decimale*10) - (long)(decimale*10);
            num_decimali--;
        }
        parte_intera.append( integer2string( decimale_long ) );
    }
    return parte_intera + std::string( (char*)&tmp[i+1] );
}

void XmlrpcResultSet::bytestringToString(xmlrpc_c::value* v, std::string* out_string) {
    xmlrpc_c::value_bytestring my_bytestring( (*v) );
    for(unsigned int i=0; i<my_bytestring.length(); i++) {
        const char c( my_bytestring.vectorUcharValue().at(i));
        out_string->push_back( c );
    }
}
void XmlrpcResultSet::structToString(xmlrpc_c::value* v, std::string* out_string) {
    xmlrpc_c::value_struct my_value( (*v) );
    std::map<std::string, xmlrpc_c::value> m = (std::map<std::string, xmlrpc_c::value>) my_value;
    out_string->append("{");
    std::map<std::string, xmlrpc_c::value>::const_iterator i;
    for(i=m.begin(); i!=m.end(); ++i) {
        if(i!=m.begin()) out_string->append(",");
        out_string->append( i->first );
        out_string->append("=>");
        valueToString( (xmlrpc_c::value*)&(i->second), out_string );
    }
    out_string->append("}");
}
void XmlrpcResultSet::arrayToString(xmlrpc_c::value* v, std::string* out_string) {
    xmlrpc_c::value_array my_value( (*v) );
    out_string->append("[");
    for(unsigned int i=0; i<my_value.size(); i++) {
        valueToString( &(my_value.vectorValueValue().at(i)), out_string );
        if(i<(my_value.size() - 1)) out_string->append(",");
    }
    out_string->append("]");
}
void XmlrpcResultSet::valueToString(xmlrpc_c::value* v, std::string* out_string) {
    switch( v->type() ) {
      case xmlrpc_c::value::TYPE_INT:
        out_string->append( integer2string( (int) xmlrpc_c::value_int(*v) ) );
        break;
      case xmlrpc_c::value::TYPE_BOOLEAN:
        out_string->append( ( (bool) xmlrpc_c::value_boolean(*v) ) ? "True":"False" );
        break;
      case xmlrpc_c::value::TYPE_DOUBLE:
        out_string->append( double2string( (double) ((float) xmlrpc_c::value_double(*v)) ) );
        break;
      case xmlrpc_c::value::TYPE_STRING:
        //out_string->append("\"");
#ifdef XMLRPCC_HAVE_CRLF
        out_string->append( xmlrpc_c::value_string(*v).crlfValue() );
#else
        out_string->append( (string) xmlrpc_c::value_string(*v) ); // BOH
#endif
        break;
      case xmlrpc_c::value::TYPE_BYTESTRING:
        bytestringToString( v , out_string );
        break;
      case xmlrpc_c::value::TYPE_ARRAY:
        arrayToString( v, out_string );
        break;
      case xmlrpc_c::value::TYPE_STRUCT:
        structToString( v, out_string );
        break;
//      case xmlrpc_c::value::TYPE_DATETIME:
//      case xmlrpc_c::value::TYPE_C_PTR:
//      case xmlrpc_c::value::TYPE_NIL:
//      case xmlrpc_c::value::TYPE_I8:
//      case xmlrpc_c::value::TYPE_DEAD:
      default:
        out_string->append( integer2string( v->type() ) );
    }
}
//********************* XmlrpcResultSet: fine.


#endif
