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

#include "qutexmlrpc/xrserver.h"

XRServer::XRServer(quint16 port, QObject* parent) : QObject(parent)
{
	server = new QTcpServer(this);
	server->listen(QHostAddress::LocalHost, port);
	connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
	_last_req = 0;
}

XRServer::~XRServer()
{
	// Sockets are deleted automatically because
	// they are childs from this object
}

QHostAddress XRServer::getPeerForRequest(int req) const
{
  QHostAddress result;
  if( _req_to_socket.contains(req) ) {
    QTcpSocket* s = _req_to_socket[req];
    result = s->peerAddress();
  }
  return result;
}

void XRServer::newConnection()
{
	QTcpSocket* s = server->nextPendingConnection();
	if (s==0)
		return;
    connect(s,SIGNAL(readyRead()),this,SLOT(readFromClient()));
	connect(s,SIGNAL(disconnected()),this,SLOT(deleteClient()));

    QString empty_headers;
    _header_strings.insert(s,empty_headers);
    _open_sockets.insert(s);
}

void XRServer::deleteClient() {
    QTcpSocket* s = (QTcpSocket*)sender();
    _open_sockets.remove(s );
    s->deleteLater();
}

void XRServer::readFromClient()
{
    QTcpSocket* s = (QTcpSocket*)( sender() );
    if ( _headers.contains(s) ) {
        if( _headers[s].contentLength() <= s->bytesAvailable() ) {
            //We are ready to go:
            parseContentAndEmit(s);
	}
    }
    else {
        //We don't have the headers for this connection yet:
        while( s->canReadLine() && (_headers.contains(s) == false) ) {
            QString this_line = s->readLine();
	    if( this_line == "\r\n" ) {
            //This is the last header line
	        _header_strings[s].append(this_line);
	        QHttpRequestHeader this_header( _header_strings[s] );
		_headers.insert(s,this_header);
		if (this_header.method() != "POST" ) {
                    //They MUST be posts:
	            sendHttpError(s,
			      405,
			      "Method Not Allowed",
			      "This is an XML-RPC server.  I only accept POST");
		}
		else if( this_header.hasContentLength() ) {
                    if( this_header.contentLength()
			<= s->bytesAvailable() ) {
                        //We are ready to go!
		        parseContentAndEmit(s);
		    }
		    //else we will exit from this function
		    //because the while condition is false.
		    //_headers.contains(s) == true now.
		}
		else {
                    //This is not a valid header
		    int this_req = _last_req++;
		    _req_to_socket.insert(this_req,s);
		    sendFault(this_req,
			      XR_SERVER_ERROR_INVALID_XMLRPC,
		              "Invalid XML-RPC headers.  Missing content length");
		}
	    }
	    else {
                _header_strings[s].append( this_line );
	    }
        }
    }
}

void XRServer::parseContentAndEmit(QTcpSocket* s)
{
	//qWarning() << s->readAll();
    XRMethodCall xrmc;
    QString error;
    //get the current request number and increment
    int this_req = _last_req++;
    _req_to_socket.insert(this_req, s);
    if( xrmc.setContent(s->readAll(),&error) ) {
	if( xrmc.parseXmlRpc() ) {
	    bool handled = false;
	    /*
	     * The slots that get this signal look to see if
	     * it is a method call they handle.  If it is,
	     * the first one that can handle it, sets handled
	     * to true.  Any slot recieving the signal with handled
	     * set to true MUST ignore it.
	     *
	     * After the methodCall signal is recieved and the call
	     * is made, one of the two slots: sendFault or sendMethodResponse
	     * must be signalled *ONLY ONE OF THEM, ONE TIME*
	     */
	    emit methodCall(this_req,
			xrmc.getMethodName(),
			xrmc.getParams(),
			handled);
	    if( !handled ) {
		//We better send a method not found error:
		//standard for fault codes:
		//http://xmlrpc-epi.sourceforge.net/specs/rfc.fault_codes.php
                sendFault(this_req,
			  XR_SERVER_ERROR_METHOD_NOT_FOUND,
		          "Method: "
				    + xrmc.getMethodName()
				    + ", not found");
	    }
	}
	else {
	   //This is a malformed XML-RPC message:
	   sendFault(this_req,
		     XR_SERVER_ERROR_INVALID_XMLRPC,
		     "Invalid XML-RPC methodCall");
	}
    }
    else {
        //There was a parse error:
	sendFault(this_req,
		  XR_PARSE_ERROR_NOT_WELL_FORMED,
		  error);
    }
}

void XRServer::sendFault(int req,
		         int fault_code,
		         const QString& fault_string)
{
  //This guarantees that each request is only answered once
  if( _req_to_socket.contains(req) ) {
    QTcpSocket* s = _req_to_socket[req];
    XRMethodResponse resp(fault_code,fault_string);
    QHttpResponseHeader heads;
    //The XML-RPC spec requires this:
    heads.setContentType("text/xml");
    sendHttpResponse(s,
		     200,
		     "OK",
		     heads,
		     resp.toString() );
    _req_to_socket.remove(req);
  }
}

void XRServer::sendHttpError(QTcpSocket* s,
		         int error_code,
			 const QString& reason,
			 const QString data)
{
    QHttpResponseHeader head;
    //We close the connection on errors.
    head.setValue("Connection","close");
    sendHttpResponse(s,error_code,reason,head,data);
}

void XRServer::sendHttpResponse(QTcpSocket* s,
		            int status_code,
			    const QString& reason,
		            QHttpHeader& headers,
			    QString resp)
{
  if( _headers.contains(s) ) {
    //If it is http 1.0 and it doesn't have keep-alive,
    //then close the connection
    if( (_headers[s].majorVersion() == 1 &&_headers[s].minorVersion() == 0) &&
        (_headers[s].value("Connection") != "keep-alive" ))
    {
        headers.setValue("Connection","close");
    }
    //This is the body of the response:
    QByteArray output = resp.toUtf8();
    int length = output.size();
    if( (length > 256) &&
	(_headers[s].value("Accept-Encoding") == "deflate")){
      headers.setValue("Content-Encoding","deflate");
      QByteArray compressed_output = qCompress(output);
      length = compressed_output.size();
      output = compressed_output;
    }
    headers.setContentLength(length);
    
    //This is the headers:
    QByteArray head_out;
	QTextStream header_resp(&head_out, QIODevice::WriteOnly);
    header_resp << "HTTP/1.1 " << status_code <<  " " << reason << "\r\n";
    QStringList::const_iterator sit;
    QStringList keys = headers.keys();
    foreach (QString sit, keys)
	{
        header_resp << sit << ": " << headers.value(sit) << "\r\n";
    }
    header_resp << "\r\n";

    //Write the headers out:
    int header_length = head_out.size();
    int tmp = 0;
    while( tmp < header_length ) {
        tmp += s->write(head_out.data() + tmp,
			              header_length - tmp);
    }
    //Write the body out:
    tmp = 0;
    while( tmp < length ) {
		tmp += s->write(output.data() + tmp,
			              length - tmp);
    }
    _headers.remove(s);
    _header_strings.remove(s);

    //Close the connection if we are supposed to
    if(headers.value("Connection") == "close" ) {
        s->close();
    }
  }
}

void XRServer::sendMethodResponse(int req, const QVariant& result)
{
  //This guarantees that each request is only answered once
  if( _req_to_socket.contains(req) ) {
    QTcpSocket* s = _req_to_socket[req];
    XRMethodResponse resp(result);
    QHttpResponseHeader heads;
    //The XML-RPC spec requires this:
    heads.setContentType("text/xml");
    sendHttpResponse(s,
		     200,
		     "OK",
		     heads,
		     resp.toString(0) );
    _req_to_socket.remove(req);
  }
    //Leave the connection open in order to save tcp setup! (http 1.1)
}
