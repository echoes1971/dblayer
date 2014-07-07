/*

qutexrlib QT-based XML-RPC library
Copyright (C) 2003  P. Oscar Boykin <boykin@pobox.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
   
*/

#ifndef xrclient
#define xrclient

#include <qurl.h>
#include <qvariant.h>
#include <QDomDocument>
#include <QNetworkAccessManager>
#include <qmap.h>
#include <qmutex.h>
#include <qbuffer.h>
#include "qutexmlrpc/xrmethodcall.h"
#include "qutexmlrpc/xrmethodresponse.h"
#include "qutexmlrpc/xrfaultcodes.h"

#include <QEventLoop>
#include <QThread>

/**
 * Represents the client which calls the methods on the remote machine.
 * @see http://www.xmlrpc.com/spec
 * @see http://www.ietf.org/internet-drafts/draft-megacz-xmc-01.txt
 */

class XmlRpcClient : public QObject {
   Q_OBJECT

  public:
    /**
     * @param server_url the URL to the server for this client
     */
    XmlRpcClient(const QUrl& server_url, QObject * parent = 0, bool debug = false, bool use_cookies = true);
    ~XmlRpcClient();
    /**
     * Allow the server to send us compressed (deflate)
     * responses.  Useful to turn this off if debugging
     * with a network sniffing tool
     */
    void acceptCompressed(bool arg) { _accept_compressed = arg; }
    /**
     * @param method the method on the server to call
     * @param params a list of the parameters
     * @return the identifier for this response
     */
    QVariant syncCall(const QString& method, const QList<QVariant>& params, const char* codecName="UTF-8");

    static QVariant staticCall(const QUrl& server_url, const QString& method, const QList<QVariant>& params, bool debug = false, const char* codecName="UTF-8");

    static QString variant2string(const QVariant& v, QString prefix="");

    // RRA: start.
    QMap<QString,QString>* getCookies();
    void setCookies(QMap<QString,QString>* cookies);
    void setDebug(bool b);
    bool isDebug();
    QNetworkReply *waitForNetworkReply(QNetworkReply* reply, int secs);
    // RRA: end.

  private:
    void processHeaders(QNetworkReply* reply);
    void processHttpResponse(QNetworkReply* reply);

  signals:
    /**
     * this signal is emitted when a NON-FAULT
     * XML-RPC response is recieved.  You must also
     * watch for the fault signals
     * @param response_num the response number for this fault
     * @param response the deserialized response to the methodCall
     * @see call
     */
    void methodResponse(int response_num, const QVariant& response);
    /**
     * @param response_num the response number for this fault
     * @param fault_num the faultCode.
     * @param fault_string the faultString
     * @see http://xmlrpc-epi.sourceforge.net/specs/rfc.fault_codes.php
     * @see call
     */
    void fault(int response_num, int fault_num, const QString& fault_string);

  protected:
    QNetworkAccessManager* _http_client;
    QUrl _url;

    bool _is_deflated;
    long _deflated_size;
    bool _accept_compressed;
    /**
     * This is the string passed in the User-Agent: header field.
     */
    static const QString USER_AGENT;

    // RRA: inizio.
    QMap<QString,QString>* cookies;
    bool debug;
    bool use_cookies;
    QVariant syncResp;
    // RRA: fine.
};

#endif
