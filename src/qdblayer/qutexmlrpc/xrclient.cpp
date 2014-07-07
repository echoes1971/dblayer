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

#include "qutexmlrpc/xrclient.h"
#include <QDebug>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTextStream>
#include <QTimer>
#include <QUrlQuery>

const QString XRClient::USER_AGENT = "QuteXmlRpc/0.0";

XRClient::XRClient(const QUrl& url, QObject * parent, bool debug, bool use_cookies) : QObject(parent),_url(url) {
    _is_deflated = false;
    _accept_compressed = true;

    this->_http_client = new QNetworkAccessManager(this);


    this->_url = url;

    // RRA: start.
    this->debug = debug;
    this->syncReq = -1;
    this->use_cookies = use_cookies;
    if(use_cookies) {
        this->cookies = new QMap<QString,QString>();
    } else {
        this->cookies = 0;
    }
    // RRA: end.
}
XRClient::~XRClient() {
    if(this->_http_client!=0) {
        this->_http_client->disconnect();//->abort();
        delete this->_http_client;
        this->_http_client = 0;
    }

    if(this->use_cookies) delete this->cookies; // RRA
}


void XRClient::processHeaders(QNetworkReply* reply) {
    if(this->debug) printf("%0lx::XRClient::processHeaders: start.\n", (unsigned long) QThread::currentThread());
    QList<QPair<QByteArray, QByteArray> > headers = reply->rawHeaderPairs();
    for(const QPair<QByteArray, QByteArray>& pair : headers) {
        if(this->debug) printf("%0lx::XRClient::processHeaders: %s=%s\n",(unsigned long) QThread::currentThread(),pair.first.data(),pair.second.data());
    }

    if(reply->hasRawHeader("Content-Encoding")) {
        if( QString(reply->rawHeader("Content-Encoding").data()) == "deflate" ) {
            _is_deflated = true;
            _deflated_size = reply->header(QNetworkRequest::ContentLengthHeader).toInt();
        } else {
            _is_deflated = false;
        }
    } else {
        _is_deflated = false;
    }

    if(this->cookies!=0 && reply->hasRawHeader("Set-Cookie")) {
        if(this->debug) printf("%0ld::XRClient::processHeaders: Set-Cookie: %s\n", (unsigned long) QThread::currentThread(), reply->rawHeader("Content-Encoding").data() );
        QStringList sl = QString(reply->rawHeader("Set-Cookie").data()).split("; ");
        for(int i=0; i<sl.size(); i++) {
            QStringList cookie = sl[i].split("=");
            if(cookie[0]=="path")
                continue;
            if(this->debug) printf("%0ld::XRClient::processHeaders: key=%s value=%s\n", (unsigned long) QThread::currentThread(), cookie[0].toStdString().c_str(),cookie[1].toStdString().c_str() );
            if(this->cookies->find(cookie[0])==this->cookies->end())
                this->cookies->insert(cookie[0],cookie[1]);
            else
                (*this->cookies)[cookie[0]]=cookie[1];
        }
    }

    if(this->debug) printf("%0lx::XRClient::processHeaders: end.\n", (unsigned long) QThread::currentThread());
}


void XRClient::processHttpResponse(QNetworkReply* reply) {
    int http_resp;
    if(this->debug) printf("%0lx::XRClient::processHttpResponse: start.\n",(unsigned long) QThread::currentThread());

    if (reply->error()) {
        if(this->debug) printf("%0lx::XRClient::processHttpResponse: error=%s\n",(unsigned long) QThread::currentThread(),reply->errorString().toStdString().c_str());
        delete reply;
        return;
    }

    QByteArray myresp = reply->readAll();

    if(this->debug) printf("%0lx::XRClient::processHttpResponse: resp=%s\n",(unsigned long) QThread::currentThread(), QString(myresp).toStdString().c_str() );
    XRMethodResponse xml_response;
    QString parse_error_string;
    bool no_parse_error;
    if(this->debug) printf("%0lx::XRClient::processHttpResponse: _is_deflated=%s\n",(unsigned long) QThread::currentThread(),(_is_deflated?"true":"false"));
    if( !_is_deflated ) {
        no_parse_error = xml_response.setContent( myresp, &parse_error_string );
    } else {
        if(this->debug) printf("%0lx::XRClient::processHttpResponse: _deflated_size=%li\n",(unsigned long) QThread::currentThread(),_deflated_size);
        myresp.insert(0,(char) _deflated_size & 0x000000ff);
        myresp.insert(0,(char) _deflated_size & 0x0000ff00);
        myresp.insert(0,(char) _deflated_size & 0x00ff0000);
        myresp.insert(0,(char) _deflated_size & 0xff000000);
        no_parse_error = xml_response.setContent( qUncompress( myresp ), &parse_error_string );
    }
    if(this->debug) printf("%0lx::XRClient::processHttpResponse: no_parse_error=%s\n",(unsigned long) QThread::currentThread(),(no_parse_error?"true":"false"));
    if( no_parse_error ) {
        if(this->debug) printf("%0lx::XRClient::processHttpResponse: xml_response.parseXmlRpc...\n",(unsigned long) QThread::currentThread());
        if( xml_response.parseXmlRpc() ) {
            int faultCode;
            QString faultString;
            if( xml_response.getFault(faultCode,faultString) ) {
                if(this->debug) printf("%0lx::XRClient::processHttpResponse: %d faultString=%s\n",(unsigned long) QThread::currentThread(),faultCode,faultString.toStdString().c_str());
                emit fault(http_resp, faultCode, faultString);
            } else {
                /* It looks good! */
                // RRA: start.
                if(this->syncReq>0) {
                    this->syncResp = xml_response.getResponse();
                    //receivedSyncResponse = true;
                } else
                // RRA: end.
                    emit methodResponse(http_resp, xml_response.getResponse() );
            }
        } else {
            /*
             * Use the standard fault codes at
             * http://xmlrpc-epi.sourceforge.net/specs/rfc.fault_codes.php.
             */
            if(this->debug) printf("%0lx::XRClient::processHttpResponse: server error: recieved bad XML-RPC grammar from remote server\n",(unsigned long) QThread::currentThread());
            emit fault(http_resp, XR_SERVER_ERROR_INVALID_XMLRPC,
                "server error: recieved bad XML-RPC grammar from remote server");
        }
    } else {
        if(this->debug) printf("%0lx::XRClient::processHttpResponse: parse_error_string=%s\n",(unsigned long) QThread::currentThread(),parse_error_string.toStdString().c_str());
        emit fault(http_resp, XR_PARSE_ERROR_NOT_WELL_FORMED, parse_error_string);
    }

    if(this->debug) printf("%0lx::XRClient::processHttpResponse: end.\n",(unsigned long) QThread::currentThread());
}

QNetworkReply* XRClient::waitForNetworkReply(QNetworkReply* reply, int secs) {
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    QTimer::singleShot(secs * 1000, &loop, SLOT(quit()));
    loop.exec();

    return reply;

//    if (reply->error()) {
//        if(this->debug) printf("%0lx::XRClient::waitForNetworkReply: error=%s\n",(unsigned long) QThread::currentThread(),reply->errorString().toStdString().c_str());
//        delete reply;
//        return "";
//    }

//    QString all = reply->readAll();
//    delete reply;
//    if (all.isEmpty()) {
//        if(this->debug) printf("%0lx::XRClient::waitForNetworkReply: all=%s\n",(unsigned long) QThread::currentThread(),all.toStdString().c_str());
//        return "";
//    }

//    return all;
}

QVariant XRClient::syncCall(const QString& method, const QList<QVariant>& params, const char* codecName) {
    if(this->debug) printf("%0lx::XRClient::syncCall: start.\n",(unsigned long) QThread::currentThread());

    //this->syncReq = this->call(method, params, codecName);
    XRMethodCall xml_method_call(method,params);

    // Serialize the request
    QByteArray payload;
    QTextStream payload_stream(&payload, QIODevice::WriteOnly);
    payload_stream.setCodec(codecName);
    //don't waste bytes on indenting
    xml_method_call.save( payload_stream, 0, QDomNode::EncodingFromTextStream);

    // Qt5 stuff :-)
    QNetworkRequest request(this->_url); //QUrl(webService() + "/api/v1/login/"));
    request.setRawHeader(QByteArray("Host"),QByteArray(_url.host().toStdString().c_str()));
    request.setHeader(QNetworkRequest::UserAgentHeader,USER_AGENT);

    // qUncompress doesn't work :-(
//    if( _accept_compressed ) {
//      // Accept deflated content
//      request.setRawHeader(QByteArray("Accept-Encoding"),QByteArray("deflate"));
//    }
    // XML-RPC *REQUIRES* the following to be set:
    request.setHeader(QNetworkRequest::ContentLengthHeader, payload.size() );
    request.setHeader(QNetworkRequest::ContentTypeHeader, "text/xml" );
    if(this->cookies!=0) {
        for(int i=0; i<this->cookies->keys().size(); i++) {
            request.setRawHeader(QByteArray("Cookie"),
                QByteArray(
                    QString("%1=%2").arg(
                        this->cookies->keys().at(i),
                        (*this->cookies)[this->cookies->keys().at(i)]
                    ).toStdString().c_str()
                )
            );
        }
    }

//    QUrlQuery parameters;
//    parameters.addQueryItem("license_string", licenseString());

    if(this->debug) printf("%0lx::XRClient::syncCall: posting request...\n",(unsigned long) QThread::currentThread());
    QNetworkReply* reply = _http_client->post(request,payload);//, parameters.query());
    //reply->ignoreSslErrors();
    if(this->debug) printf("%0lx::XRClient::syncCall: call waitForNetworkReply...\n",(unsigned long) QThread::currentThread());
    reply = waitForNetworkReply(reply, 120);

    processHeaders(reply);
    processHttpResponse(reply);

    if(this->debug) printf("%0lx::XRClient::syncCall: this->syncReq=-1...\n",(unsigned long) QThread::currentThread());
    this->syncReq=-1;
    if(this->debug) printf("%0lx::XRClient::syncCall: ret = this->syncResp...\n",(unsigned long) QThread::currentThread());
    QVariant ret = this->syncResp;

    if(this->debug) printf("%0lx::XRClient::syncCall: end.\n",(unsigned long) QThread::currentThread());
    return ret;
}

QMap<QString,QString>* XRClient::getCookies() { return this->cookies; }
void XRClient::setCookies(QMap<QString,QString>* cookies) { this->cookies=cookies; }
void XRClient::setDebug(bool b) { this->debug=b; }
bool XRClient::isDebug() { return this->debug; }

QVariant XRClient::staticCall(const QUrl& server_url, const QString& method, const QList<QVariant>& params, bool debug, const char* codecName) {
    static QMap<QString,QString> staticCookies;
    XRClient* c = new XRClient(server_url, 0, debug);
    c->acceptCompressed(false);
    c->setCookies(&staticCookies);
    QVariant ret = c->syncCall(method,params,codecName);
    delete c;
    return ret;
}
