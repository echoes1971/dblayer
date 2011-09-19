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

/*
SQLite Types:
- NULL
- INTEGER
- FLOAT
- TEXT
- BLOB


CREATE TABLE test_dblayer
(
  id int NOT NULL,
  nome text,
  descrizione text,
  abilitato int,
  data_creazione text,
  prezzo float,
  data_disponibilita text,
  PRIMARY KEY (id)
);
create index test_dblayer_idx_1 on test_dblayer ( id );

CREATE TABLE societa
(
  id integer NOT NULL,
  data_creazione text,
  ragione_sociale text,
  indirizzo text,
  cap integer,
  nazione text,
  telefono integer,
  fax integer,
  email text,
  note text,
  website text,
  citta text,
  provincia text,
  partita_iva text,
  tipo text,
  PRIMARY KEY (id)
);
create index societa_idx_1 on societa ( id );

CREATE TABLE rra_societa
(
  id integer NOT NULL,
  data_creazione text,
  ragione_sociale text,
  indirizzo text,
  cap integer,
  nazione text,
  telefono integer,
  fax integer,
  email text,
  note text,
  website text,
  citta text,
  provincia text,
  partita_iva text,
  tipo text,
  PRIMARY KEY (id)
);
create index societa_idx_1 on rra_societa ( id );


*/




namespace DBLayer {

#ifdef USE_LIBSQLITE3
	class DLLEXPORT SQLiteConnection : public Connection {
	  public:
		SQLiteConnection(string s);
		virtual ~SQLiteConnection();

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
		sqlite3* db;
		static int sqliteCallback(void *NotUsed, int argc, char **argv, char **azColName);

	};


	class DLLEXPORT SQLiteResultSet : public ResultSet {
		friend class SQLiteConnection;
		private:
//			StringVector columnName;
//			StringVector columnType;
//			StringVector righe;
		public:
			SQLiteResultSet();
			virtual ~SQLiteResultSet();
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
