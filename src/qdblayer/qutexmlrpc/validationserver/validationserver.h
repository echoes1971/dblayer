#include "xrserver.h"

/**
 * An example XRServer which implements the validation spec.
 * @see http://www.xmlrpc.com/validator1Docs
 */

class XRValidationServer : public XRServer {
 
    Q_OBJECT
    public:
        XRValidationServer(quint16 port=8080,
                 int backlog = 1,
                 QObject * parent = 0,
                 const char * name = 0 );    
    
    public slots:
        void arrayOfStructsTest(
		    int req,
                    const QString& methodname,
                    const QVariantList& params,
                    bool& handled);
    
        void countTheEntities(
		    int req,
                    const QString& methodname,
                    const QVariantList& params,
                    bool& handled);
        void easyStructTest(
		    int req,
                    const QString& methodname,
                    const QVariantList& params,
                    bool& handled);
	void echoStructTest(
		    int req,
                    const QString& methodname,
                    const QVariantList& params,
                    bool& handled);
	void manyTypesTest(
		    int req,
                    const QString& methodname,
                    const QVariantList& params,
                    bool& handled);
	void moderateSizeArrayCheck(
		    int req,
                    const QString& methodname,
                    const QVariantList& params,
                    bool& handled);
        void nestedStructTest(	
		    int req,
                    const QString& methodname,
                    const QVariantList& params,
                    bool& handled);
	void simpleStructReturnTest(
		    int req,
                    const QString& methodname,
                    const QVariantList& params,
                    bool& handled);
	void sumAndDifference(
		    int req,
                    const QString& methodname,
                    const QVariantList& params,
                    bool& handled);
};
