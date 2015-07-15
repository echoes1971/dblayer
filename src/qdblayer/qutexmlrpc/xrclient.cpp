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

#include <iostream>
using namespace std;

const QString XmlRpcClient::USER_AGENT = "QuteXmlRpc/0.0";

XmlRpcClient::XmlRpcClient(const QUrl& url, QObject * parent, bool debug, bool use_cookies) : QObject(parent),_url(url) {
    _is_deflated = false;
    _accept_compressed = true;

    this->_http_client = new QNetworkAccessManager(this);

    this->_url = url;

    // RRA: start.
    this->debug = debug;
    this->use_cookies = use_cookies;
    if(use_cookies) {
        this->cookies = new QMap<QString,QString>();
    } else {
        this->cookies = 0;
    }

    this->_logger =  [] (const string& s, const bool newline) -> void {
        cout << s;
        if(newline) cout << endl;
    };
    // RRA: end.
}
XmlRpcClient::~XmlRpcClient() {
    if(this->_http_client!=0) {
        this->_http_client->disconnect();//->abort();
        delete this->_http_client;
        this->_http_client = 0;
    }

    if(this->use_cookies) delete this->cookies; // RRA
}


void XmlRpcClient::processHeaders(QNetworkReply* reply) {
    if(this->debug) this->log(QString("%1::XmlRpcClient::processHeaders: start.").arg((unsigned long) QThread::currentThread()));
    if(this->debug) {
        QList<QPair<QByteArray, QByteArray> > headers = reply->rawHeaderPairs();
        for(const QPair<QByteArray, QByteArray>& pair : headers) {
            this->log(QString("%1::XmlRpcClient::processHeaders: %2=%3").arg((unsigned long) QThread::currentThread()).arg(pair.first.data()).arg(pair.second.data()));
        }
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
        if(this->debug) this->log(QString("%1::XmlRpcClient::processHeaders: Set-Cookie: %2\n").arg((unsigned long) QThread::currentThread()).arg(reply->rawHeader("Content-Encoding").data()) );
        QStringList sl = QString(reply->rawHeader("Set-Cookie").data()).split("; ");
        for(int i=0; i<sl.size(); i++) {
            QStringList cookie = sl[i].split("=");
            if(cookie[0]=="path")
                continue;
            if(this->debug) this->log(QString("%1::XmlRpcClient::processHeaders: key=%2 value=%3").arg((unsigned long) QThread::currentThread()).arg(cookie[0]).arg(cookie[1]) );
            if(this->cookies->find(cookie[0])==this->cookies->end())
                this->cookies->insert(cookie[0],cookie[1]);
            else
                (*this->cookies)[cookie[0]]=cookie[1];
        }
    }

    if(this->debug) this->log(QString("%1::XmlRpcClient::processHeaders: end.").arg((unsigned long) QThread::currentThread()));
}


void XmlRpcClient::processHttpResponse(QNetworkReply* reply) {
    int http_resp = 0;
    if(this->debug) this->log(QString("%1::XmlRpcClient::processHttpResponse: start.\n").arg((unsigned long) QThread::currentThread()));

    if (reply->error()) {
        if(this->debug) this->log(QString("%1::XmlRpcClient::processHttpResponse: error=%2").arg((unsigned long) QThread::currentThread()).arg(reply->errorString()));
        delete reply;
        return;
    }

    QByteArray myresp = reply->readAll();

    if(this->debug) this->log(QString("%1::XmlRpcClient::processHttpResponse: resp=%2").arg((unsigned long) QThread::currentThread()).arg(QString(myresp.data())));
    XmlRpcMethodResponse xml_response;
    QString parse_error_string;
    bool no_parse_error;
    if(this->debug) this->log(QString("%1::XmlRpcClient::processHttpResponse: _is_deflated=%2").arg((unsigned long) QThread::currentThread()).arg(_is_deflated?"true":"false"));
    if( !_is_deflated ) {
        no_parse_error = xml_response.setContent( myresp, &parse_error_string );
    } else {
        if(this->debug) this->log(QString("%1::XmlRpcClient::processHttpResponse: _deflated_size=%2").arg((unsigned long) QThread::currentThread()).arg(_deflated_size));
        myresp.insert(0,(char) _deflated_size & 0x000000ff);
        myresp.insert(0,(char) _deflated_size & 0x0000ff00);
        myresp.insert(0,(char) _deflated_size & 0x00ff0000);
        myresp.insert(0,(char) _deflated_size & 0xff000000);
        no_parse_error = xml_response.setContent( qUncompress( myresp ), &parse_error_string );
    }
    if(this->debug) this->log(QString("%1::XmlRpcClient::processHttpResponse: no_parse_error=%2").arg((unsigned long) QThread::currentThread()).arg(no_parse_error?"true":"false"));
    if( no_parse_error ) {
        if(this->debug) this->log(QString("%1::XmlRpcClient::processHttpResponse: xml_response.parseXmlRpc...\n").arg((unsigned long) QThread::currentThread()));
        if( xml_response.parseXmlRpc() ) {
            int faultCode;
            QString faultString;
            if( xml_response.getFault(faultCode,faultString) ) {
                if(this->debug) this->log(QString("%1::XmlRpcClient::processHttpResponse: %2 faultString=%3").arg((unsigned long) QThread::currentThread()).arg(faultCode).arg(faultString));
                emit fault(http_resp, faultCode, faultString);
            } else {
                this->syncResp = xml_response.getResponse();
                //receivedSyncResponse = true;
                emit methodResponse(http_resp, xml_response.getResponse() );
            }
        } else {
            /*
             * Use the standard fault codes at
             * http://xmlrpc-epi.sourceforge.net/specs/rfc.fault_codes.php.
             */
            if(this->debug) this->log(QString("%1::XmlRpcClient::processHttpResponse: server error: recieved bad XML-RPC grammar from remote server\n").arg((unsigned long) QThread::currentThread()));
            emit fault(http_resp, XR_SERVER_ERROR_INVALID_XMLRPC,
                "server error: recieved bad XML-RPC grammar from remote server");
        }
    } else {
        if(this->debug) this->log(QString("%1::XmlRpcClient::processHttpResponse: parse_error_string=%2").arg((unsigned long) QThread::currentThread()).arg(parse_error_string));
        emit fault(http_resp, XR_PARSE_ERROR_NOT_WELL_FORMED, parse_error_string);
    }

    if(this->debug) this->log(QString("%1::XmlRpcClient::processHttpResponse: end.\n").arg((unsigned long) QThread::currentThread()));
}

QNetworkReply* XmlRpcClient::waitForNetworkReply(QNetworkReply* reply, int secs) {
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    QTimer::singleShot(secs * 1000, &loop, SLOT(quit()));
    loop.exec();

    return reply;

//    if (reply->error()) {
//        if(this->debug) this->log(QString("%1::XmlRpcClient::waitForNetworkReply: error=%2").arg((unsigned long) QThread::currentThread()).arg(reply->errorString()));
//        delete reply;
//        return "";
//    }

//    QString all = reply->readAll();
//    delete reply;
//    if (all.isEmpty()) {
//        if(this->debug) this->log(QString("%1::XmlRpcClient::waitForNetworkReply: all=%2").arg((unsigned long) QThread::currentThread()).arg(all));
//        return "";
//    }

//    return all;
}

QVariant XmlRpcClient::syncCall(const QString& method, const QList<QVariant>& params, int wait_seconds, const char* codecName) {
    if(this->debug) this->log(QString("%1::XmlRpcClient::syncCall: start.\n").arg((unsigned long) QThread::currentThread()));

    //this->syncReq = this->call(method, params, codecName);
    XmlRpcMethodCall xml_method_call(method,params);

    // Serialize the request
    QByteArray payload;
    QTextStream payload_stream(&payload, QIODevice::WriteOnly);
    payload_stream.setCodec(codecName);
    //don't waste bytes on indenting
    xml_method_call.save( payload_stream, 0, QDomNode::EncodingFromTextStream);

    // Qt5 stuff :-)
    if(this->debug) this->log(QString("%1::XmlRpcClient::syncCall: url=%2\n").arg((unsigned long) QThread::currentThread()).arg(this->_url.toString()));
    QNetworkRequest request(this->_url);
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

    if(this->debug) this->log(QString("%1::XmlRpcClient::syncCall: posting request...\n").arg((unsigned long) QThread::currentThread()));
    this->syncResp.clear();
    QNetworkReply* reply = _http_client->post(request,payload);//, parameters.query());
    //reply->ignoreSslErrors();
    if(this->debug) this->log(QString("%1::XmlRpcClient::syncCall: call waitForNetworkReply...\n").arg((unsigned long) QThread::currentThread()));
    reply = waitForNetworkReply(reply, wait_seconds);

    processHeaders(reply);
    processHttpResponse(reply);

    if(this->debug) this->log(QString("%1::XmlRpcClient::syncCall: ret = this->syncResp...\n").arg((unsigned long) QThread::currentThread()));
    QVariant ret = this->syncResp;

    if(this->debug) this->log(QString("%1::XmlRpcClient::syncCall: end.\n").arg((unsigned long) QThread::currentThread()));
    return ret;
}

QMap<QString,QString>* XmlRpcClient::getCookies() { return this->cookies; }
void XmlRpcClient::setCookies(QMap<QString,QString>* cookies) { this->cookies=cookies; }
void XmlRpcClient::setDebug(bool b) { this->debug=b; }
bool XmlRpcClient::isDebug() { return this->debug; }

QVariant XmlRpcClient::staticCall(const QUrl& server_url, const QString& method, const QList<QVariant>& params, bool debug, int wait_seconds, const char* codecName) {
    static QMap<QString,QString> staticCookies;
    XmlRpcClient* c = new XmlRpcClient(server_url, 0, debug);
    c->acceptCompressed(false);
    c->setCookies(&staticCookies);
    QVariant ret = c->syncCall(method,params,wait_seconds,codecName);
    delete c;
    return ret;
}

QString XmlRpcClient::variant2string(const QVariant& v, QString prefix) {
    QString ret;
    QStringList l;
    QList<QString> chiavi;
    switch( v.type() ) {
        case QVariant::ByteArray:
            ret = prefix + QString("<value type=\"") + QString(v.typeName()) + QString("\" bytes=\"") + QString("%1").arg(v.toByteArray().size()) + QString("\">") + v.toString() + QString("</value>");
            break;
        case QVariant::List:
            l.clear();
            for(int i=0; i<v.toList().size(); i++)
                l.append( variant2string(v.toList().at(i), prefix+QString("  ")) );
            ret = prefix + QString("<list>")
                  + l.join(QString())
                  + prefix + QString("</list>");
            break;
        case QVariant::Map:
            chiavi.clear();
            chiavi = v.toMap().keys();
            for(int i=0; i<chiavi.size(); i++)
                l.append( prefix+QString("  ")+QString("<key>")+chiavi.at(i)+QString("</key>")
                          +prefix+QString("  ")+QString("<value>")
                          +variant2string(v.toMap().value(chiavi.at(i)), prefix+QString("  "))
                          +prefix+QString("  ")+QString("</value>")
                          );
            ret = prefix+QString("<map>")
                  + l.join(QString())
                  + prefix+QString("</map>");
            break;
        default:
            ret = prefix + QString("<value type=\"") + QString(v.typeName()) + QString("\">") + v.toString() + QString("</value>");
            break;
    }
    return ret;
}


void XmlRpcClient::setLogger(std::function<void(const string&,bool)> logger) { this->_logger = logger; }
void XmlRpcClient::log(const string& s, const bool newline) const { this->_logger(s,newline); }
void XmlRpcClient::log(const QString& s) { this->log(s.toStdString()); }
