/***************************************************************************
**	sqliteconnection.h  v0.1.2 - 2006.05.22
**	---------------------------------------
**
**	Author:		Roberto Rocco Angeloni.
**	email:		roberto@roccoangeloni.it
**	Comment:	Implementazione di connection e resultset per SQLite
**	To Do:		- SQLiteResultSet::getColumnType: RICONOSCERE SE E' UNA DATA!!!
**	Future:
**	History:
**		v0.1.0 - 2006.05.11 Iniziato lo sviluppo
**		v0.1.1 - 2006.05.14 Completato SQLiteResultSet e SQLiteConnection ad
**				 eccezione di alcuni metodi che non sono supportati in sqlite3
**
** @copyright &copy; 2011 by Roberto Rocco Angeloni <roberto@roccoangeloni.it>
** @license http://opensource.org/licenses/lgpl-3.0.html GNU Lesser General Public License, version 3.0 (LGPLv3)
** @version $Id: mysqlconnection.h $
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

#ifndef DBLAYER_MYSQLCONNECTION_H
#define DBLAYER_MYSQLCONNECTION_H

#include "importedPackages.h"
#include "dblayer.h"
#include "dbconnection.h"
using namespace DBLayer;

namespace DBLayer {

#ifdef USE_MYSQL
    class DECLSPECIFIER MySQLConnection : public Connection {
    public:
        MySQLConnection(string s);
        virtual ~MySQLConnection();

        virtual bool connect();
        virtual bool disconnect();

        virtual ResultSet* exec(const string s);
        /** Chiude la connessione corrente e la riapre */
        virtual bool reconnect();

        virtual string escapeString(string s);

        virtual int getColumnSize(string* relname);
        virtual string getColumnName(string* relname, int column);
        virtual IntegerVector getKeys(string* relname);
        virtual IntegerVector getForeignKeys(string* relname);

    private:
        MYSQL* db;
        string host;
        string dbname;
        string user;
        string password;

        /** Solo per debug */
        void printField(MYSQL_FIELD* field);

        static string getNomeTipo(st_mysql_field* field);
    };

    class DECLSPECIFIER MySQLResultSet : public ResultSet {
        friend class MySQLConnection;
      private:
      public:
            MySQLResultSet();
            virtual ~MySQLResultSet();

            virtual string toString(string prefix="\n");
    };
#endif
}

#endif
