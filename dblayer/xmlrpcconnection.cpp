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
#include <config.h>

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
// 	cout << "XmlrpcConnection::connect: connectionString=" << this->connectionString << endl;
	this->errorMessage.clear();
	try {
		this->_client.call(this->connectionString, "selectAsArray", "ss", &result, "nometabella"
			, "show tables"
// 			, "select * from rra_users"
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
	
// 	std::string out_string;
// 	XmlrpcResultSet::valueToString( &(myarray.vectorValueValue().at(0)), &out_string );
// 	std::cout << "XmlrpcConnection::connect: Messaggi=" << out_string << std::endl;
	
// 	out_string.clear();
// 	XmlrpcResultSet::valueToString( &(myarray.vectorValueValue().at(1)), &out_string );
// 	std::cout << "XmlrpcConnection::connect: Lista=" << out_string << std::endl;
	
	return true;
}
bool XmlrpcConnection::disconnect() {
	return true;
}
bool XmlrpcConnection::reconnect() {
// 	if( this->disconnect() )
		return this->connect();
// 	else
// 		return false;
}

bool debugXmlrpcExec = false;

ResultSet* XmlrpcConnection::exec(const string s) {
	XmlrpcResultSet* rs = new XmlrpcResultSet();
	xmlrpc_c::value result;
	cout << "XmlrpcConnection::exec: s=" << this->connectionString << endl;
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
	
	if(debugXmlrpcExec)	{
		std::string out_string;
		XmlrpcResultSet::valueToString( &(myarray.vectorValueValue().at(0)), &out_string );
		std::cout << "XmlrpcConnection::connect: Messaggi=" << out_string << std::endl;
		
		out_string.clear();
		XmlrpcResultSet::valueToString( &(myarray.vectorValueValue().at(1)), &out_string );
		std::cout << "XmlrpcConnection::connect: Lista=" << out_string << std::endl;
	}
	xmlrpc_c::value_array lista(myarray.vectorValueValue().at(1));
	// Map
	std::map<std::string, xmlrpc_c::value> m;
	// Iterator
	std::map<std::string, xmlrpc_c::value>::const_iterator i;
	for(unsigned int r=0; r<lista.size(); r++) {
		if(debugXmlrpcExec) {
			std::string out_string;
			XmlrpcResultSet::valueToString( &(lista.vectorValueValue().at(r)), &out_string );
			std::cout << "XmlrpcConnection::connect: " << r << ") " << out_string << std::endl;
		}
		// Struct
		xmlrpc_c::value_struct v( lista.vectorValueValue().at(r) );
		// Map
		m = (std::map<std::string, xmlrpc_c::value>) v;
		unsigned int colonna=0;
		string my_string;
		for(i=m.begin(); i!=m.end(); ++i) {
			// Metadati
			if(rs->columnName.size()<=colonna) rs->columnName.push_back( i->first );
			my_string.clear();
			switch(i->second.type()) {
				case xmlrpc_c::value::TYPE_INT:
					if(rs->columnType.size()<=colonna) rs->columnType.push_back( DBLayer::type_integer );
					rs->righe.push_back( XmlrpcResultSet::integer2string( (int) xmlrpc_c::value_int(i->second) ) );
					break;
				case xmlrpc_c::value::TYPE_BOOLEAN:
					if(rs->columnType.size()<=colonna) rs->columnType.push_back( DBLayer::type_integer );
					rs->righe.push_back( XmlrpcResultSet::integer2string( ( (bool) xmlrpc_c::value_boolean(i->second) ) ? 1 : 0 ) );
					break;
				case xmlrpc_c::value::TYPE_DOUBLE:
					if(rs->columnType.size()<=colonna) rs->columnType.push_back( DBLayer::type_double );
					rs->righe.push_back( XmlrpcResultSet::double2string( (double) ((float) xmlrpc_c::value_double(i->second)) ) );
					break;
				case xmlrpc_c::value::TYPE_STRING:
					if(rs->columnType.size()<=colonna) rs->columnType.push_back( DBLayer::type_string );
					rs->righe.push_back( xmlrpc_c::value_string(i->second) );
					break;
				case xmlrpc_c::value::TYPE_BYTESTRING:
					if(rs->columnType.size()<=colonna) rs->columnType.push_back( DBLayer::type_string );
					XmlrpcResultSet::bytestringToString( &xmlrpc_c::value_bytestring( i->second ), &my_string );
					rs->righe.push_back( my_string );
					break;
// 				default:
// 					case xmlrpc_c::value::TYPE_ARRAY:
// 					case xmlrpc_c::value::TYPE_STRUCT:
// 					case xmlrpc_c::value::TYPE_DATETIME:
// 					case xmlrpc_c::value::TYPE_C_PTR:
// 					case xmlrpc_c::value::TYPE_NIL:
// 					case xmlrpc_c::value::TYPE_I8:
// 					case xmlrpc_c::value::TYPE_DEAD:
				default:
					if(rs->columnType.size()<=colonna) rs->columnType.push_back( DBLayer::type_blob );
					XmlrpcResultSet::valueToString( &xmlrpc_c::value(i->second), &my_string );
					rs->righe.push_back( my_string );
					break;
			}
			colonna++;
		}
	}
	
/*
	int errorCode;
	const char* query = s.c_str();
	if(debugXmlrpcExec)	cout << "XmlrpcConnection::exec: query = " << query << endl;

	cout << "XmlrpcConnection::exec: rs = " << rs << endl;
	this->errorMessage.clear();

	// Preparo i metadati
	if(debugXmlrpcExec)	cout << "XmlrpcConnection::exec: Preparo i metadati " << endl;
// 	int nColonne = sqlite3_column_count(pStmt);
	if(debugXmlrpcExec)	cout << "XmlrpcConnection::exec: nColonne=" << DBLayer::integer2string((long)nColonne) << endl;
	for( int i=0; i<nColonne; i++) {
// 		const char* nomeColonna = sqlite3_column_name(pStmt,i);
		cout << "XmlrpcConnection::exec: nomeColonna = " << nomeColonna << endl;
		rs->columnName.push_back( string(nomeColonna) );
//		delete nomeColonna;
		const char* nomeTipo = 0;//sqlite3_column_decltype(pStmt,i);
		if(nomeTipo!=0) {
			if(debugXmlrpcExec)	cout << "XmlrpcConnection::exec: nomeTipo=\'" << nomeTipo << "\'" << endl;
			if( strcmp(nomeTipo,"int")==0 || strcmp(nomeTipo,"integer")==0) {
				rs->columnType.push_back( DBLayer::type_integer );
			} else if( strcmp(nomeTipo,"text"  )==0 ) {
				rs->columnType.push_back( DBLayer::type_string );
			} else if( strcmp(nomeTipo,"float"  )==0 ) {
				rs->columnType.push_back( DBLayer::type_double );
			} else if( strcmp(nomeTipo,"blob"  )==0 ) {
				rs->columnType.push_back( DBLayer::type_blob );
			} else {
				cout << "XmlrpcConnection::exec: codiceTipo \'" << nomeTipo
					 << "\' NON riconosciuto!" << endl;
				rs->columnType.push_back( DBLayer::type_blob );
			}
		} else {
			cout << "XmlrpcConnection::exec: nomeTipo VUOTO" << endl;
			rs->columnType.push_back( DBLayer::type_blob );
		}
//		delete nomeTipo;
	}

	// Leggo la prima riga
	errorCode = 0;//sqlite3_step(pStmt);
	if(debugXmlrpcExec)	cout << "XmlrpcConnection::exec: Leggo la prima riga " << endl;

	// Leggo tutte le righe
	if(debugXmlrpcExec)	cout << "XmlrpcConnection::exec: Leggo tutte le righe " << endl;
	while( errorCode==SQLITE_ROW ) {
		for(unsigned int c=0; c<rs->columnName.size(); c++) {
			int sizeDato = sqlite3_column_bytes(pStmt,c);
			// SE il tipo e' blob, allora probabilmente non e' stato identificato
			// inizialmente, quindi lo aggiorniamo mentre leggiamo la riga
			if( rs->columnType[c]==DBLayer::type_blob ) {
				int tipoDiDato = sqlite3_column_type(pStmt,c);
				switch( tipoDiDato ) {
					case SQLITE_INTEGER:
						rs->columnType[c]=DBLayer::type_integer;
						break;
					case SQLITE_FLOAT:
						rs->columnType[c]=DBLayer::type_double;
						break;
					case SQLITE_TEXT:
						rs->columnType[c]=DBLayer::type_string;
						break;
				}
			}

			if(debugXmlrpcExec)	cout << "XmlrpcConnection::exec: sizeDato=\'" << DBLayer::integer2string((long)sizeDato) << "\'" << endl;
			if(sizeDato>0) {
				char* tmpValore = new char[ sqlite3_column_bytes(pStmt,c) * 2 ];
				const unsigned char* tmpSorgente = sqlite3_column_text(pStmt,c);
				sprintf( tmpValore, "%s", tmpSorgente );
				if(debugXmlrpcExec)	cout << "XmlrpcConnection::exec: tmpValore=\'" << tmpValore << "\'" << endl;
				string valore = string( tmpValore );
				rs->righe.push_back( valore );
				delete tmpValore;
				//delete tmpSorgente;
			} else {
				rs->righe.push_back( string("\\N") );
			}
		}
		errorCode = sqlite3_step(pStmt);
	}
	if( errorCode!=SQLITE_DONE ) {
		cout << "XmlrpcConnection::exec: errorCode = " << errorCode << endl;
		this->errorMessage.append(
			"errorCode: "+ DBLayer::integer2string((long)errorCode) +
			"; msg: " + sqlite3_errmsg(this->db)
		);
		cout << "XmlrpcConnection::exec: errorMessage = " << this->errorMessage << endl;
	}
	if(debugXmlrpcExec)	cout << "XmlrpcConnection::exec: Leggo tutte le righe... OK! " << endl;


	if(debugXmlrpcExec)	cout << "XmlrpcConnection::exec: finalizzo..." << endl;
// 	errorCode = sqlite3_finalize( pStmt );
	if(debugXmlrpcExec)	cout << "XmlrpcConnection::exec: finalizzo... OK" << endl;
	if( errorCode!=SQLITE_OK ) {
		cout << "XmlrpcConnection::exec: errorCode = " << errorCode << endl;
		this->errorMessage.append(
			"errorCode: "+ DBLayer::integer2string((long)errorCode) +
			"; msg: " + sqlite3_errmsg(this->db)
		);
		cout << "XmlrpcConnection::exec: errorMessage = " << this->errorMessage << endl;
	} else {
		if(debugXmlrpcExec)	cout << "XmlrpcConnection::exec: errorCode==SQLITE_OK" << endl;
	}
	cout << "XmlrpcConnection::exec: query = " << query << endl;
//	delete query;

	cout << "XmlrpcConnection::exec: rs=" << rs->toString() << endl;
	cout << "XmlrpcConnection::exec: fine." << endl;
*/
	return rs;
}

string XmlrpcConnection::escapeString(string s) {
	static string fromQuote("\'");
	static string   toQuote("\'\'");
	return DBLayer::replaceAll(s, fromQuote, toQuote);
}

int XmlrpcConnection::getColumnSize(string* relname) {
    return -1;
}
string XmlrpcConnection::getColumnName(string* relname, int column) {
	string ret="";
	return ret;
}
IntegerVector XmlrpcConnection::getKeys(string* relname) {
	IntegerVector ret;
    cout << "XmlrpcConnection::getKeys: UNSUPPORTED; relname=" << relname << endl;
	return ret;
}
IntegerVector XmlrpcConnection::getForeignKeys(string* relname) {
	IntegerVector ret;
    cout << "XmlrpcConnection::getForeignKeys: UNSUPPORTED; relname=" << relname << endl;
	return ret;
}
//********************* XmlrpcConnection: fine.



//********************* XmlrpcResultSet: inizio.

#ifdef WIN32
XmlrpcResultSet::XmlrpcResultSet() : ResultSet() {
#else
XmlrpcResultSet::XmlrpcResultSet() : ResultSet::ResultSet() {
#endif
}
XmlrpcResultSet::~XmlrpcResultSet() {
//	cout << "XmlrpcResultSet::~XmlrpcResultSet: inizio." << endl;
//	cout << "XmlrpcResultSet::~XmlrpcResultSet: fine." << endl;
}

//int XmlrpcResultSet::getNumColumns() {
//	return this->columnName.size();
//}
//int XmlrpcResultSet::getNumRows() {
//	if( this->columnName.size()!=0 )
//		return this->righe.size() / this->columnName.size() ;
//	else
//		return 0;
//}

//string XmlrpcResultSet::getColumnName(int i) {
//	return this->columnName[i];
//}
//string XmlrpcResultSet::getColumnType(int i) {
//	return this->columnType[i];
//}
int XmlrpcResultSet::getColumnSize(int i) {
	// Non significativo per Xmlrpc
    return -i;
//	return -1;
}
//int XmlrpcResultSet::getColumnIndex(string* columnName ) {
//	int ret = -1;
//	const char* columnNameChar = columnName->c_str();
//	for(unsigned int i=0; i<this->columnName.size() && ret<0; i++) {
//		const char* currentColumnNameChar = this->columnName[i].c_str();
//		if( strcmp( columnNameChar, currentColumnNameChar )==0 )
//			ret = i;
//	}
//	return ret;
//}

//string XmlrpcResultSet::getValue(int row, int column) {
//	return this->righe.at( row * this->columnName.size() + column );
//}
//bool XmlrpcResultSet::isNull(int row, int column) {
//	string tmp = this->getValue(row,column);
//	return tmp=="\\N";// || tmp.size()==0;
//}
//int XmlrpcResultSet::getLength(int row, int column) {
//	return this->righe.at( row * this->columnName.size() + column ).size();
//}
string XmlrpcResultSet::toString(string prefix) {
	string ret;
	ret.append(prefix+"<XmlrpcResultSet>");

//	cout << "XmlrpcResultSet::toString: Stampo le colonne." << endl;
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

//	cout << "XmlrpcResultSet::toString: Stampo le righe." << endl;
	ret.append(prefix+" <Rows>");
	int nRighe = this->getNumRows();
	for(int r=0; r<nRighe; r++) {
//		cout << "XmlrpcResultSet::toString: riga = " << DBLayer::integer2string((long)r) << endl;
		ret.append(prefix+"  <Row num=\'"+DBLayer::integer2string((long)r)+"\'>");
		for(int c=0; c<nColonne; c++) {
			string nomeColonna = this->getColumnName(c);
//			cout << "XmlrpcResultSet::toString: nomeColonna = " << nomeColonna << endl;
//			cout << "XmlrpcResultSet::toString: valoreColonna = " << this->getValue(r,c) << endl;
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
};

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

void XmlrpcResultSet::bytestringToString(xmlrpc_c::value_bytestring* v, std::string* out_string) {
// 	out_string->append("\"");
    for(unsigned int i=0; i<v->length(); i++) {
		const char c( v->vectorUcharValue().at(i));
		out_string->push_back( c );
	}
// 	out_string->append("\"");
}

void XmlrpcResultSet::structToString(xmlrpc_c::value_struct* v, std::string* out_string) {
	std::map<std::string, xmlrpc_c::value> m = (std::map<std::string, xmlrpc_c::value>) (*v);
	out_string->append("{");
	std::map<std::string, xmlrpc_c::value>::const_iterator i;
	for(i=m.begin(); i!=m.end(); ++i) {
		if(i!=m.begin()) out_string->append(",");
// 		out_string->append("\"");
		out_string->append( i->first );
// 		out_string->append("\"");
		out_string->append("=>");
		valueToString( (xmlrpc_c::value*)&(i->second), out_string );
	}
	out_string->append("}");
}

void XmlrpcResultSet::arrayToString(xmlrpc_c::value_array* v, std::string* out_string) {
	out_string->append("[");
	for(unsigned int i=0; i<v->size(); i++) {
		valueToString( &(v->vectorValueValue().at(i)), out_string );
		if(i<(v->size() - 1)) out_string->append(",");
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
// 			out_string->append("\"");
			out_string->append( xmlrpc_c::value_string(*v).crlfValue() );
// 			out_string->append("\"");
			break;
		case xmlrpc_c::value::TYPE_BYTESTRING:
			bytestringToString( &xmlrpc_c::value_bytestring( (*v) ), out_string );
			break;
		case xmlrpc_c::value::TYPE_ARRAY:
			arrayToString( &xmlrpc_c::value_array( (*v) ), out_string );
			break;
		case xmlrpc_c::value::TYPE_STRUCT:
			structToString( &xmlrpc_c::value_struct( (*v) ), out_string );
			break;
		case xmlrpc_c::value::TYPE_DATETIME:
		case xmlrpc_c::value::TYPE_C_PTR:
		case xmlrpc_c::value::TYPE_NIL:
		case xmlrpc_c::value::TYPE_I8:
		case xmlrpc_c::value::TYPE_DEAD:
		default:
			out_string->append( integer2string( v->type() ) );
	}
}
//********************* XmlrpcResultSet: fine.



#endif
