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
   
#ifndef xrmethodcall
#define xrmethodcall

#include <qstring.h>
#include <QList>
#include <qvariant.h>
#include <qdom.h>
#include <xrvariant.h>

/**
 * Represents the XML structure for XML-RPC method calls.
 * Handles serialization and deserialization through QDomDocument.
 */

class XRMethodCall : public QDomDocument {
	
    public:
	/**
	 * Constructor used when you are deserializing via QDomDocument
	 * methods, such as QDomDocument::setContent.
	 * After using such a method to set the content, you must
	 * call parseXmlRpc, which will make sure the xml has a valid
	 * grammar.
	 */
	XRMethodCall();

	/**
	 * @param name the method name for this call.
	 * @param params The array of parameters for the call
	 */
        XRMethodCall(const QString& name, const QList<QVariant>& params);
	/**
	 * This method MUST be called each time and QDomDocument
	 * method is used to change the document.
	 * For instance, after a copy constructor from QDomDocument,
	 * or after any setContent method calls, etc...
	 * @return true if the XML is a valid xml-rpc message */
	bool parseXmlRpc();

	QString getMethodName() const { return _method_name; }
	const QList<QVariant>& getParams() const { return _params; }
	
    protected:
	static bool fromDomDoc(const QDomDocument& doc,
			       QString& method_name,
		  QList<QVariant>& params);

        static void toDomDoc(const QString& method_name,
			     const QList<QVariant>& params,
			     QDomDocument& doc);

	QString _method_name;
	QList<QVariant> _params;
};

#endif
