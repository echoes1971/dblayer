/***************************************************************************
**	odbcconnection.h  v0.1.2 - 2012.03.19
**	-----------------------------------
**
**	Author:		Roberto Rocco Angeloni.
**	E-mail:		roberto@roccoangeloni.it
**	Comment:	Implementazione di connection e resultset per ODBC
**	To Do:		- ...
**	Future:
**	History:
**		v0.1.0 - 2006.05.21 Iniziato lo sviluppo
**		v0.1.1 - 2006.05.22 Terminato lo sviluppo
**				In ODBCResultSet e' stata riciclata l'implementazione
**				di ResultSet. ODBCConnection ricalca SQLiteConnection.
**				Gestite le date di Access: quando viene creata la connessione
**				di determina anche il dbmsName, in base a questo il metodo
**				<b>quoteDate</b> mette la data tra <b>#</b> piuttosto che tra <b>'</b>
**		v0.1.2 - 2006.05.23 Adattato per Linux
**
** @copyright &copy; 2011-2014 by Roberto Rocco Angeloni <roberto@roccoangeloni.it>
** @license http://opensource.org/licenses/lgpl-3.0.html GNU Lesser General Public License, version 3.0 (LGPLv3)
** @version $Id: odbcconnection.h $
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

#ifndef DBLAYER_ODBCCONNECTION_H
#define DBLAYER_ODBCCONNECTION_H

#include "importedPackages.h"
#include "dblayer.h"
#include "dbconnection.h"
using namespace DBLayer;

#define MAX_DATA 100

namespace DBLayer {

#ifdef USE_ODBCPP

	class DECLSPECIFIER ODBCConnection : public Connection {
	  public:
		ODBCConnection(string s);
		virtual ~ODBCConnection();

		virtual bool connect();
		virtual bool disconnect();

		virtual ResultSet* exec(const string s);

		virtual string quoteDate(string s);

        inline string getDBType() { return "ODBC"; }

	  private:
		RETCODE rc;		// ODBC return code
		SQLHENV henv;	// Environment
		SQLHDBC hdbc;	// Connection handle
		HSTMT hstmt;	// Statement Handle
#ifdef __WIN32
		unsigned char chr_ds_name[SQL_MAX_DSN_LENGTH];   // Data source name
#else
		char chr_ds_name[SQL_MAX_DSN_LENGTH];   // Data source name
#endif
		bool connectionEstablished;
		string dbmsName;

		inline bool isRCSuccessful();
		void saveErrorMessage();
        /**	Decode column type */
		inline string decodeType(SQLSMALLINT mytype);
	};


	class DECLSPECIFIER ODBCResultSet : public ResultSet {
		friend class ODBCConnection;
      private:
		public:
			ODBCResultSet();
			virtual ~ODBCResultSet();
    };

#endif

}

#endif
