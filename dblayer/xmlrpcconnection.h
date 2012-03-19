/***************************************************************************
**	xmlrpcconnection.h  v0.1.0 - 2012.03.19
**	-----------------------------------
**
**	Author:		Roberto Rocco Angeloni.
**	E-mail:		roberto@roccoangeloni.it
**	Comment:	Implementazione di connection e resultset per Xmlrpc
**	To Do:		- 
**	Future:
**	History:
**		v0.1.0 - 2006.05.11 Iniziato lo sviluppo
**
** @copyright &copy; 2011-2012 by Roberto Rocco Angeloni <roberto@roccoangeloni.it>
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


namespace DBLayer {

#ifdef USE_LIBXMLRPC

class XmlrpcResultSet;

class DECLSPECIFIER XmlrpcConnection : public Connection {
  public:
    XmlrpcConnection(string s);
    virtual ~XmlrpcConnection();

    virtual bool connect();
    virtual bool disconnect();

    virtual ResultSet* exec(const string s);
    virtual bool reconnect();

    virtual string escapeString(string s);

    virtual int getColumnSize(string* relname);
    virtual string getColumnName(string* relname, int column);
    virtual IntegerVector getKeys(string* relname);
    virtual IntegerVector getForeignKeys(string* relname);

    virtual ResultSet* login(string user, string pwd);

    static XmlrpcResultSet* list2resultset(xmlrpc_c::value_array* iLista, XmlrpcResultSet* ioResultSet);

  private:
    xmlrpc_c::clientSimple _client;

};


class DECLSPECIFIER XmlrpcResultSet : public ResultSet {
    friend class XmlrpcConnection;
  private:
    static void valueToString(xmlrpc_c::value* v, std::string* out_string);
    static std::string integer2string(long longValue);
    static std::string double2string(double longValue);
    static void bytestringToString(xmlrpc_c::value* v, std::string* out_string);
    static void structToString(xmlrpc_c::value* v, std::string* out_string);
    static void arrayToString(xmlrpc_c::value* v, std::string* out_string);
  public:
    XmlrpcResultSet();
    virtual ~XmlrpcResultSet();
    virtual int getColumnSize(int i);

    virtual string toString(string prefix="\n");
};
#endif
}

#endif
