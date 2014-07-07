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

#include "qutexmlrpc/xrmethodresponse.h"
#include <QDebug>

XmlRpcMethodResponse::XmlRpcMethodResponse() : QDomDocument(),
	                                    _is_fault(false),
					    _fault_code(0),
					    _fault_string(),
					    _response()
{

}

XmlRpcMethodResponse::XmlRpcMethodResponse(const QVariant& resp) :
                    QDomDocument(), _is_fault(false), _fault_code(0),
		    _fault_string(), _response(resp)
{
  
  responseToDomDoc(_response,*this);
}

XmlRpcMethodResponse::XmlRpcMethodResponse(int fault_code,
		                        const QString& string) :
                          QDomDocument(), _is_fault(true),
			  _fault_code(fault_code), _fault_string(string),
			  _response()
{
    faultToDomDoc(_fault_code,_fault_string,*this);
}

void XmlRpcMethodResponse::faultToDomDoc(int fault_code, const QString& fault_string, QDomDocument& doc) {
    QDomElement root = doc.createElement("methodResponse");
    doc.appendChild(root);
    
    QDomElement fault = doc.createElement("fault");
    root.appendChild(fault);
    
    QMap<QString,QVariant> fault_map;
    fault_map.insert("faultCode",QVariant(fault_code));
    fault_map.insert("faultString", QVariant(fault_string));
    
    fault.appendChild( XmlRpcVariant(fault_map).toDomElement(doc) );
}

bool XmlRpcMethodResponse::fromDomDoc(const QDomDocument& doc, QVariant& result, bool& isFault) {
//   QDomNodeList nodes;
   QDomElement docElem = doc.documentElement();
   if( docElem.tagName() != "methodResponse" ) {
     return false;
   }
   QDomNode n = docElem.firstChild();
   while( !n.isNull() ) {
     QDomElement e = n.toElement();
     if( !e.isNull() ) {
       if( e.tagName() == "fault" ) {
         QDomNode fault_value = e.firstChild();
         QDomElement fault_e = fault_value.toElement();
         XmlRpcVariant fault(fault_e);
         if( fault.type() == QVariant::Map ) {
           QMap<QString, QVariant> faultMap = fault.toMap();
           if( faultMap.find("faultCode") != faultMap.end() && faultMap.find("faultString") != faultMap.end() ) {
               isFault = true;
               result = fault;
               return true;
           }
         } else {
           return false;
         }
       }
       if( e.tagName() == "params") {
         QDomNode p = e.firstChild();
         if( !p.isNull() ) {
           QDomElement param = p.toElement();
           if( param.tagName() == "param") {
             QDomNode value = param.firstChild();
             if( value.isNull() ) {
               return false;
             } else {
               QDomElement value_e = value.toElement();
               result = XmlRpcVariant(value_e);
             }
           }
         } else {
           return false;
         }
       }
     }
     n = n.nextSibling();
   }
   return true;
}

bool XmlRpcMethodResponse::getFault(int& code, QString& string) const {
  if( _is_fault ) {
    code = _fault_code;
    string = _fault_string;
    return true;
  } else {
    return false;
  }
}

bool XmlRpcMethodResponse::parseXmlRpc() {
   bool result = fromDomDoc(*this,_response,_is_fault);
   if( _is_fault ) {
	   QMap<QString,QVariant>::ConstIterator it;
	   QMap<QString,QVariant> _responseMap = _response.toMap();
	   it = _responseMap.find("faultCode");
       _fault_code = it.value().toInt();
	   it = _responseMap.find("faultString");
	   _fault_string = it.value().toString();
       _response.clear();
   }
   return result;
}

void XmlRpcMethodResponse::responseToDomDoc(const QVariant& result, QDomDocument& doc) {
    QDomElement root = doc.createElement("methodResponse");
	QDomElement params_xml = doc.createElement("params");
    QDomElement param_node = doc.createElement("param");
	param_node.appendChild( XmlRpcVariant(result).toDomElement(doc) );
	params_xml.appendChild(param_node);	
	root.appendChild(params_xml);
	doc.appendChild(root);
}
