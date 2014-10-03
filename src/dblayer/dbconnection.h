/***************************************************************************
**    dbconnection.h  v0.1.2 - 2012.03.19
**    -----------------------------------
**
**    Author:        Roberto Rocco Angeloni.
**    E-mail:        roberto@roccoangeloni.it
**    Comment:    Implementazione di connection e resultset
**    To Do:        - getForeignKeys
**    Future:
**    History:
**        v0.1.0 - 2006.05.09 Prima versione stabile
**        v0.1.1 - 2006.05.13 Modificata la createConnection, ora e' possibile
**                    passare una stringa di connessione del tipo
**                    "dblayer:sqlite3:c:\tmp\test.db"
**                    "dblayer:pg:host=localhost dbname=roberto user=roberto password="
**                    :-) :-) :-) :-) :-) :-) :-) :-) :-) :-) :-) :-) :-) :-)
**        v0.1.2 - 2006.05.22 Implementato ResultSet come SQLiteResultSet, ovvero con
**                    l'utilizzo di vettori di stringhe per memorizzare i risultati
**                    Aggiunto -quoteDate-
**        v0.1.3 - 2007.04.20 Implementato MySQLResultSet e MySQLConnection
**
** @copyright &copy; 2011-2014 by Roberto Rocco Angeloni <roberto@roccoangeloni.it>
** @license http://opensource.org/licenses/lgpl-3.0.html GNU Lesser General Public License, version 3.0 (LGPLv3)
** @version $Id: dbconnection.h $
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

#ifndef DBLAYER_DBCONNECTION_H
#define DBLAYER_DBCONNECTION_H

#include "importedPackages.h"
#include "dblayer.h"
using namespace DBLayer;

namespace DBLayer {
    static string type_boolean = string("boolean");
    static string type_integer = string("integer");
    static string type_double = string("double");
    static string type_string = string("string");
    static string type_datetime = string("datetime");
    static string type_blob = string("blob");

    static const string db_mysql="mysql";
    static string db_odbc = string("odbc");
    static string db_pg = string("pg");
    static string db_sqlite = string("sqlite");
    static string db_xmlrpc = string("xmlrpc");
    static string db_type = db_pg;

    class DECLSPECIFIER ResultSet {
        protected:
            StringVector  columnName;
            StringVector  columnType;
            IntegerVector columnSize;
            StringVector righe;
        public:
            ResultSet();
            virtual ~ResultSet();

            virtual int getNumColumns() const;
            virtual int getNumRows() const;
            virtual string getValue(int row, int column) const;
            virtual string getValue(int row, string* columnName) const;
            virtual int getLength(int row, int column) const;
            virtual bool isNull(int row, int column) const;
            virtual string getColumnName(int i) const;
            virtual int getColumnIndex(string* columnName ) const;
            virtual string getColumnType(int i) const;
            virtual int getColumnSize(int i) const;
            virtual string getErrorMessage() const;
            bool hasErrors() const;
            virtual string getStatus() const;
            /**    A scopo di debug...    */
            virtual string toString(string prefix="\n") const;
    };

#ifdef USE_LIBPQ
    class DECLSPECIFIER PGResultSet : public ResultSet {
        private:
            PGresult* res;
        public:
            PGResultSet();
            PGResultSet(PGresult* res);
            virtual ~PGResultSet();
            virtual int getNumColumns() const;
            virtual int getNumRows() const;
            virtual string getValue(int row, int column) const;
            virtual int getLength(int row, int column) const;
            virtual bool isNull(int row, int column) const;
            virtual string getColumnName(int i) const;
            virtual int getColumnIndex(string* columnName ) const;
            virtual string getColumnType(int i) const;
            virtual int getColumnSize(int i) const;
            virtual string getErrorMessage() const;
            virtual string getStatus() const;
    };
#endif

    class DECLSPECIFIER Connection;
    typedef map<string,Connection* (*)(string s)> ConnectionBuildersMap; // 20090811

    class DECLSPECIFIER Connection {
        private:
            // Nothing upto now
        protected:
            bool connected;
            bool verbose;
            string connectionString;
            string errorMessage;
        public:
            Connection(string s);
            virtual ~Connection();

            string getErrorMessage() const;
            bool hasErrors() const;

            void setVerbose(bool b);
            bool isVerbose() const;

            virtual bool connect();
            virtual bool disconnect();
            bool isConnected() const;

            inline const string getConnectionString() { return this->connectionString; }

            virtual ResultSet* login(string user, string pwd);

            virtual ResultSet* exec(const string s);
            /** Force the write buffer to be written (or at least try) */
            virtual bool flush();
            /** Chiude la connessione corrente e la riapre */
            virtual bool reconnect();
            virtual string* escapeString(string* s) const;
            virtual string escapeString(string s) const;
            virtual string quoteDate(string s) const;

            virtual ColumnDefinitions getColumnsForTable(const string& tablename);
            /** Ritorna il numero di colonne di una tabella */
            virtual int getColumnSize(string* relname);
            /** @param column 1..n */
            virtual string getColumnName(string* relname, int column);
            /** Ritorna il numero delle colonne chiave della tabella  [1..n] */
            virtual IntegerVector getKeys(string* relname);
            /** Ritorna il numero delle colonne chiave esterne della tabella [1..n] */
            virtual IntegerVector getForeignKeys(string* relname);
            /** Ritorna i nomi delle colonne chiave della tabella */
            StringVector getKeysNames(string* relname);

            virtual string getFormSchema(string language="python");
            virtual string getDBSchema(string language="python");
            virtual string getSchemaName() const;
            virtual string getDBType() const;

            virtual string dbeType2dbType(const string& dbetype) const;
            virtual string dbType2dbeType(const string& dbtype) const;
            virtual string dbConstraints2dbeConstraints(map<string,string>& def) const;
            virtual string dbColumnDefinition2dbeColumnDefinition(map<string,string>& def) const;

            // **************** Proxy Connections: start. *********************
            // The proxy connections are used by DBMgr to execute the following methods
            virtual bool isProxy() const;
            virtual DBEntity* Insert(DBEntity *dbe);
            virtual DBEntity* Update(DBEntity *dbe);
            virtual DBEntity* Delete(DBEntity *dbe);
            virtual DBEntityVector* Select(const DBEntity* dbe, const string& tableName, const string& searchString);
            virtual DBEntityVector* Search(DBEntity* dbe, bool uselike=true,
                                    bool caseSensitive=true, const string& orderBy="" );
            virtual string ping();
            // **************** Proxy Connections: end. *********************

    };

#ifdef USE_LIBPQ
    class DECLSPECIFIER PGConnection : public Connection {
            private:
                PGconn* conn;
            public:
                PGConnection(string s);
                ~PGConnection();

        // Overridden: start.
        bool connect();
        bool disconnect();
        ResultSet* exec(const string s);
                /** Force the write buffer to be written (or at least try) */
        bool flush();
                /** Chiude la connessione corrente e la riapre */
        bool reconnect();

        string escapeString(string s);

        ColumnDefinitions getColumnsForTable(const string& tablename);
        int getColumnSize(string* relname);
        string getColumnName(string* relname, int column);
        IntegerVector getKeys(string* relname);

        inline string getDBType() const { return "POSTGRESQL"; }
        
        virtual string dbeType2dbType(const string& dbetype) const;
        // Overridden: end.

                // PG Functions
        static string pgtype2string(int t);
                int clientEncoding();
                int setClientEncoding(string s);
    };
#endif

    /**
     *    Parameters: connection string
     *    The connection string could be the normal connection string for
     *    the default database or can contain the prefix of the desided dbms
     *    with the following syntax:
     *        [dblayer:<dbmsprefix>:]connectionString
     *    dbmsprefix available: sqlite, pg
     *
     *    Examples:
     *        "dblayer:odbc:mydnsstring"
     *        "dblayer:sqlite3:c:\tmp\test.db"
     *        "dblayer:pg:host=localhost dbname=roberto user=roberto password="
     */
    DECLSPECIFIER Connection* createConnection(string s);

    extern ConnectionBuildersMap connectionBuilders;
    void registerConnectionType(string prefix, Connection* (*myBuilder)(string s));
}

#endif
