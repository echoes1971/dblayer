/***************************************************************************
**	xmlrpcconnection.h  v0.1.0 - 2009.05.19
**	---------------------------------------
**
**	Author:		Roberto Rocco Angeloni.
**	email:		roberto@roccoangeloni.it
**	Comment:	Implementazione di connection e resultset per Xmlrpc
**	To Do:		- 
**	Future:
**	History:
**		v0.1.0 - 2006.05.11 Iniziato lo sviluppo
**
** @copyright &copy; 2011 by Roberto Rocco Angeloni <roberto@roccoangeloni.it>
** @license http://opensource.org/licenses/lgpl-3.0.html GNU Lesser General Public License, version 3.0 (LGPLv3)
** @version $Id: xmlrpcconnection.h $
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

#ifndef DBLAYER_XMLRPCCONNECTION_H
#define DBLAYER_XMLRPCCONNECTION_H

#include "importedPackages.h"
#include "dblayer.h"
#include "dbconnection.h"
using namespace DBLayer;

/*

*/




namespace DBLayer {

#ifdef USE_LIBXMLRPC
	class DLLEXPORT XmlrpcConnection : public Connection {
	  public:
		XmlrpcConnection(string s);
		virtual ~XmlrpcConnection();
		
		virtual bool connect();
		virtual bool disconnect();
		
		virtual ResultSet* exec(const string s);
		/** Force the write buffer to be written (or at least try) */
//		virtual bool flush();
		/** Chiude la connessione corrente e la riapre */
		virtual bool reconnect();
		
		virtual string escapeString(string s);
		
		virtual int getColumnSize(string* relname);
		virtual string getColumnName(string* relname, int column);
		virtual IntegerVector getKeys(string* relname);
		virtual IntegerVector getForeignKeys(string* relname);
		
	  private:
		xmlrpc_c::clientSimple _client;
// 		sqlite3* db;
// 		static int sqliteCallback(void *NotUsed, int argc, char **argv, char **azColName);

	};


	class DLLEXPORT XmlrpcResultSet : public ResultSet {
		friend class XmlrpcConnection;
		private:
//			StringVector columnName;
//			StringVector columnType;
//			StringVector righe;
			static void valueToString(xmlrpc_c::value* v, std::string* out_string);
			static std::string integer2string(long longValue);
			static std::string double2string(double longValue);
			static void bytestringToString(xmlrpc_c::value_bytestring* v, std::string* out_string);
			static void structToString(xmlrpc_c::value_struct* v, std::string* out_string);
			static void arrayToString(xmlrpc_c::value_array* v, std::string* out_string);
		public:
			XmlrpcResultSet();
			virtual ~XmlrpcResultSet();
//			virtual int getNumColumns();
//			virtual int getNumRows();
//			virtual string getValue(int row, int column);
//			virtual int getLength(int row, int column);
//			virtual bool isNull(int row, int column);
//			virtual string getColumnName(int i);
//			virtual int getColumnIndex(string* columnName );
//			virtual string getColumnType(int i);
			virtual int getColumnSize(int i);

//			virtual string getErrorMessage();
//			virtual string getStatus();

			virtual string toString(string prefix="\n");
	};
#endif
}

#endif
