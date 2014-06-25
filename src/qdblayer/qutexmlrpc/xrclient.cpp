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
#include <QTextStream>
#include <QDebug>

const QString XRClient::USER_AGENT = "QuteXmlRpc/0.0";

XRClient::XRClient(const QUrl& url, QObject * parent, bool debug, bool use_cookies) : QObject(parent),_url(url) {
    _is_deflated = false;
    _accept_compressed = true;

    this->_http_client = new QHttp(this);

    if( url.port()!=-1 ) {
        _http_client->setHost(url.host(), url.port());
    } else {
        _http_client->setHost(url.host());
    }

    /*
     * connect some slots to signals!
     */
    connect( _http_client, SIGNAL(requestFinished(int,bool)),
           this, SLOT(processHttpResponse(int, bool)));//, Qt::DirectConnection );
    connect( _http_client,
          SIGNAL(responseHeaderReceived(const QHttpResponseHeader &)),
           this, SLOT(processHeaders(const QHttpResponseHeader &)));//, Qt::QueuedConnection );

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
    /* tell the http client to abort all on-going requests
     * this may result in the slot processHttpResponse()
     * being called for the current request.
     */
    if(this->_http_client!=0) {
        this->_http_client->abort();
        delete this->_http_client;
        this->_http_client = 0;
    }

    /* now we must delete all the buffers */
    QMap<int,QBuffer*>::iterator it;
    for(it = this->_buffer_map.begin(); it != this->_buffer_map.end(); it++) {
        delete it.value();
    }

    if(this->use_cookies) delete this->cookies; // RRA
}

void XRClient::setUrl(const QUrl& url) {
    this->_url = url;
    if( url.port()!=-1 ) {
        this->_http_client->setHost(url.host(), url.port());
    } else {
        this->_http_client->setHost(url.host());
    }
}
void XRClient::setUrl(const QString& u) {
    this->_url = u;
    if( this->_url.port()!=-1 ) {
        this->_http_client->setHost(this->_url.host(), this->_url.port());
    } else {
        this->_http_client->setHost(this->_url.host());
    }
}


int XRClient::call(const QString& method, const QList<QVariant>& params, const char* codecName) {
    if(this->debug) printf("%0ld::XRClient::call: start.\n", (unsigned long) QThread::currentThread());
    XRMethodCall xml_method_call(method,params);
    
    // Serialize the request
    QByteArray payload;
    QTextStream payload_stream(&payload, QIODevice::WriteOnly);
    payload_stream.setCodec(codecName);
    //don't waste bytes on indenting
    xml_method_call.save( payload_stream, 0, QDomNode::EncodingFromTextStream);

    /* make the buffer and make the call */
    QHttpRequestHeader req_head("POST",_url.path());
    req_head.setValue("Host",_url.host());
    req_head.setValue("User-Agent",USER_AGENT);
    if( _accept_compressed ) {
      /* Accept deflated content */
      req_head.setValue("Accept-Encoding","deflate");
    }
    /* XML-RPC *REQUIRES* the following to be set: */
    req_head.setContentLength( payload.size() );
    req_head.setContentType("text/xml");
    
    // RRA: start.
    if(this->cookies!=0)
        for(int i=0; i<this->cookies->keys().size(); i++) {
            req_head.setValue("Cookie",
                              QString("%1=%2").arg(
                                      this->cookies->keys().at(i),
                                      (*this->cookies)[this->cookies->keys().at(i)]
                                      )
                              );
        }
    // RRA: end.

    /* this buffer will hold all the result.  We don't
     * care about seeing any data until we have the whole result
     */
    QBuffer* this_buffer = new QBuffer();
    int http_req_num = this->_http_client->request(req_head, payload, this_buffer);

    this->_buffer_map.insert(http_req_num, this_buffer);

    if(this->debug) printf("%0ld::XRClient::call: http_req_num=%d\n", (unsigned long) QThread::currentThread(), http_req_num);
    if(this->debug) printf("%0ld::XRClient::call: end.\n", (unsigned long) QThread::currentThread());
    return http_req_num;
}

void XRClient::processHeaders(const QHttpResponseHeader & resp) {
    if(this->debug) printf("%0ld::XRClient::processHeaders: resp=%s\n", (unsigned long) QThread::currentThread(), resp.toString().toStdString().c_str() );
//    if(this->debug) printf("%0x::XRClient::processHeaders: resp=%s\n", (unsigned int) QThread::currentThread(), resp.toString().toStdString().c_str() );
    if( resp.hasKey("Content-Encoding") ) {
        if( resp.value("Content-Encoding") == "deflate" ) {
            _is_deflated = true;
        } else {
            _is_deflated = false;
        }
    } else {
        _is_deflated = false;
    }
    // RRA: start.
    if(this->cookies!=0 && resp.hasKey("Set-Cookie")) {
        if(this->debug) printf("%0ld::XRClient::processHeaders: Set-Cookie: %s\n", (unsigned long) QThread::currentThread(), resp.value("Set-Cookie").toStdString().c_str() );
//        if(this->debug) printf("%0x::XRClient::processHeaders: Set-Cookie: %s\n", (unsigned int) QThread::currentThread(), resp.value("Set-Cookie").toStdString().c_str() );
        QStringList sl = resp.value("Set-Cookie").split("; ");
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
    // RRA: end.
    if(this->debug) printf("%0lx::XRClient::processHeaders: end.\n", (unsigned long) QThread::currentThread());
//    if(this->debug) printf("%0x::XRClient::processHeaders: end.\n", (unsigned int) QThread::currentThread());
}
void XRClient::processHttpResponse(int http_resp, bool error) {
    if(this->debug) printf("%0lx::XRClient::processHttpResponse: start.\n",(unsigned long) QThread::currentThread());
    bool receivedSyncResponse = true;
    if( error ) {
        /*
         * Use the standard fault codes at
         * http://xmlrpc-epi.sourceforge.net/specs/rfc.fault_codes.php.
         */
        if(this->debug) printf("%0lx::XRClient::processHttpResponse: error=%s\n",(unsigned long) QThread::currentThread(),_http_client->errorString().toStdString().c_str());
//        if(this->debug) printf("%0x::XRClient::processHttpResponse: error=%s\n",(unsigned int) QThread::currentThread(),_http_client->errorString().toStdString().c_str());
        emit fault(http_resp, XR_TRANSPORT_ERROR, _http_client->errorString() );
    } else {
       if( _buffer_map.contains(http_resp) ) {
            if(this->debug) printf("%0lx::XRClient::processHttpResponse: resp=%s\n",(unsigned long) QThread::currentThread(), QString(_buffer_map[http_resp]->buffer()).toStdString().c_str() );
            XRMethodResponse xml_response;
            QString parse_error_string;
            bool no_parse_error;
            if( !_is_deflated ) {
                no_parse_error = xml_response.setContent( _buffer_map[http_resp]->buffer(), &parse_error_string );
            } else {
                /* this is compressed content */
                no_parse_error = xml_response.setContent( qUncompress( _buffer_map[http_resp]->buffer() ), &parse_error_string );
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
        } else {
            //QHttp seems to emit a signal as soon as it is created.
            //So, just ignore it if we didn't make it.
            receivedSyncResponse = false;
        }
    }
  
    /* clean up the memory for the buffer here */
    if(this->debug) printf("%0lx::XRClient::processHttpResponse: cleaning up...\n",(unsigned long) QThread::currentThread());
    if( _buffer_map.contains( http_resp ) ) {
        delete _buffer_map[http_resp];
        _buffer_map.remove(http_resp);
    }
    if(receivedSyncResponse && this->syncReq>0) this->myloop.exit(); // RRA
    if(this->debug) printf("%0lx::XRClient::processHttpResponse: end.\n",(unsigned long) QThread::currentThread());
}

QVariant XRClient::syncCall(const QString& method, const QList<QVariant>& params, const char* codecName) {
    if(this->debug) printf("%0lx::XRClient::syncCall: start.\n",(unsigned long) QThread::currentThread());

    if(this->debug) printf("%0lx::XRClient::syncCall: this->call...\n",(unsigned long) QThread::currentThread());
    this->syncReq = this->call(method, params, codecName);
    if(this->debug) printf("%0lx::XRClient::syncCall: this->syncReq=%d\n",(unsigned long) QThread::currentThread(), this->syncReq);
    this->myloop.exec();

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

QVariant XRClient::threadSafeSyncCall(const QString& method, const QList<QVariant>& params, const char* codecName) {
    //XRClient* c = new XRClient(this->_url, this, this->debug, false);
    XRClient* c = new XRClient(this->_url, 0, this->debug, false);
    c->acceptCompressed(false);
    c->setCookies(this->getCookies());
    QVariant ret = c->syncCall(method,params,codecName);
    delete c;
    return ret;
}
