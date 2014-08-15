/***************************************************************************
**	sqliteconnection.h  v0.1.2 - 2012.03.19
**	-----------------------------------
**
**	Author:		Roberto Rocco Angeloni.
**	E-mail:		roberto@roccoangeloni.it
**	Comment:	Implementazione di connection e resultset per SQLite
**	To Do:		- SQLiteResultSet::getColumnType: RICONOSCERE SE E' UNA DATA!!!
**	Future:
**	History:
**		v0.1.0 - 2006.05.11 Iniziato lo sviluppo
**		v0.1.1 - 2006.05.14 Completato SQLiteResultSet e SQLiteConnection ad
**				 eccezione di alcuni metodi che non sono supportati in sqlite3
**
** @copyright &copy; 2011-2014 by Roberto Rocco Angeloni <roberto@roccoangeloni.it>
** @license http://opensource.org/licenses/lgpl-3.0.html GNU Lesser General Public License, version 3.0 (LGPLv3)
** @version $Id: sqliteconnection.h $
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

#ifndef DBLAYER_SQLITECONNECTION_H
#define DBLAYER_SQLITECONNECTION_H

#include "importedPackages.h"
#include "dblayer.h"
#include "dbconnection.h"
using namespace DBLayer;

namespace DBLayer {

#ifdef USE_LIBSQLITE3
	class DECLSPECIFIER SQLiteConnection : public Connection {
	  public:
		SQLiteConnection(string s);
		virtual ~SQLiteConnection();

    // Override: start.
    bool connect();
    bool disconnect();

    ResultSet* exec(const string s);
    /** Chiude la connessione corrente e la riapre */
    bool reconnect();

    string escapeString(string s);

    ColumnDefinitions getColumnsForTable(const string& tablename);
    int getColumnSize(string* relname);
    string getColumnName(string* relname, int column);
    IntegerVector getKeys(string* relname);
    IntegerVector getForeignKeys(string* relname);
    inline string getDBType() { return "SQLite"; }

    virtual string dbeType2dbType(const string& dbetype);
    // Override: end.

	  private:
		sqlite3* db;
		static int sqliteCallback(void *NotUsed, int argc, char **argv, char **azColName);

	};


	class DECLSPECIFIER SQLiteResultSet : public ResultSet {
		friend class SQLiteConnection;
		private:
		public:
			SQLiteResultSet();
			virtual ~SQLiteResultSet();
			virtual int getColumnSize(int i);

			virtual string toString(string prefix="\n");
	};
#endif
}

#endif
