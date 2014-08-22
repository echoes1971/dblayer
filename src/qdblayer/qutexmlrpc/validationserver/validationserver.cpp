#include "validationserver.h"

XRValidationServer::XRValidationServer(quint16 port,
                 int backlog,
                 QObject * parent,
                 const char * name) : XRServer(port,parent)
{

  /*
   * This looks horrific, but it is really simple (just long).
   * This wires up ALL slots to the methodCall signal.
   */
	
  connect(this,
		  SIGNAL(methodCall(int,
				    const QString&,
				    const QVariantList&,
				    bool&)),
          this,
	   SLOT(arrayOfStructsTest(int,
			   const QString&,
			   const QVariantList&,
			   bool&)));
  
  connect(this,
		  SIGNAL(methodCall(int,
				  const QString&,
				  const QVariantList&,
				  bool&)),
		  this,
		  SLOT(countTheEntities(int,
				  const QString&,
				  const QVariantList&,
				  bool&)));
  
  connect(this,
          SIGNAL(methodCall(int,
			    const QString&,
			    const QVariantList&,
			    bool&)),
	  SLOT(easyStructTest(int,
			      const QString&,
			      const QVariantList&,
			      bool&)));
  
  connect(this,
	  SIGNAL(methodCall(int,
			    const QString&,
			    const QVariantList&,
			    bool&)),
	  SLOT(echoStructTest(int,
			      const QString&,
			      const QVariantList&,
			      bool&)));
  
  connect(this,
          SIGNAL(methodCall(int,
			    const QString&,
			    const QVariantList&,
			    bool&)),
	  SLOT(manyTypesTest(int,
			     const QString&,
			     const QVariantList&,
			     bool&)));
  
  connect(this,
	  SIGNAL(methodCall(int,
			    const QString&,
			    const QVariantList&,
			    bool&)),
	  SLOT(moderateSizeArrayCheck(int,
			              const QString&,
				      const QVariantList&,
				      bool&)));
  connect(this,
	  SIGNAL(methodCall(int,
			    const QString&,
			    const QVariantList&,
			    bool&)),
	  SLOT(nestedStructTest(int,
			        const QString&,
				const QVariantList&,
				bool&)));
  
  connect(this,
          SIGNAL(methodCall(int,
			    const QString&,
			    const QVariantList&,
			    bool&)),
	  SLOT(simpleStructReturnTest(int,
			    const QString&,
			    const QVariantList&,
			    bool&)));
  
  connect(this,
          SIGNAL(methodCall(
			    int,
			    const QString&,
			    const QVariantList&,
			    bool&)),
	  SLOT(sumAndDifference(
			    int,
			    const QString&,
			    const QVariantList&,
			    bool&))
			  );
}

void XRValidationServer::arrayOfStructsTest(
                    int req,
                    const QString& methodname,
                    const QVariantList& params,
                    bool& handled
		    )
{
 if( (handled == false) &&
      (methodname == "validator1.arrayOfStructsTest") ) {
  handled = true;
  //Here we through caution to the wind, assume all the params are okay
  QVariantList array = params[0].toList();
  QMap<QString, QVariant> map;
  QVariantList::iterator it;
  int result = 0;
  
  for(it = array.begin(); it != array.end(); it++) {
    map = (*it).toMap();
    if( map.contains("curly") ) {
      result += map["curly"].toInt();  
    }
  }
  sendMethodResponse(req,QVariant(result));
 }
}

void XRValidationServer::countTheEntities(
		    int req,
                    const QString& methodname,
                    const QVariantList& params,
                    bool& handled)
{
    if( (handled == false)
        && (methodname == "validator1.countTheEntities") ) {
  
	handled = true;
        
	QMap<QString, QVariant> result;
	QString input = params[0].toString();
	
	result["ctLeftAngleBrackets"] = QVariant(input.contains('<'));
	result["ctRightAngleBrackets"] = QVariant(input.contains('>'));
	result["ctAmpersands"] = QVariant(input.contains('&'));
	result["ctApostrophes"] = QVariant(input.contains('\''));
	result["ctQuotes"] = QVariant(input.contains('"'));

	sendMethodResponse(req,QVariant(result));
    }
}

void XRValidationServer::easyStructTest(
		    int req,
                    const QString& methodname,
                    const QVariantList& params,
                    bool& handled)
{
    if( (handled == false)
        && (methodname == "validator1.easyStructTest") ) {

        handled = true;
        int result = 0;
	QMap<QString, QVariant> map = params[0].toMap();
	
	result += map["larry"].toInt();
	result += map["moe"].toInt();
	result += map["curly"].toInt();
	
	sendMethodResponse(req,QVariant(result));
    }
}

void XRValidationServer::echoStructTest(
		    int req,
                    const QString& methodname,
                    const QVariantList& params,
                    bool& handled)
{
    if( (handled == false)
	&& (methodname == "validator1.echoStructTest") )
    {
        handled = true;
        sendMethodResponse(req, QVariant( params[0].toMap() ) );
    }
}

void XRValidationServer::manyTypesTest(
		    int req,
                    const QString& methodname,
                    const QVariantList& params,
                    bool& handled)
{
    if( (handled == false)
	&& (methodname == "validator1.manyTypesTest") )
    {
        handled = true;
        sendMethodResponse(req, QVariant( params ) );
    }
}

void XRValidationServer::moderateSizeArrayCheck(
		    int req,
                    const QString& methodname,
                    const QVariantList& params,
                    bool& handled)
{
    if( (handled == false)
        && (methodname == "validator1.moderateSizeArrayCheck") )
    {
        handled = true;
        QVariantList array = params[0].toList();
	QString result = array.front().toString() + array.back().toString();
	
        sendMethodResponse(req, QVariant( result ) );
    }
}

void XRValidationServer::nestedStructTest(
                    int req,
		    const QString& methodname,
                    const QVariantList& params,
                    bool& handled)
{
    if( (handled == false)
        && (methodname == "validator1.nestedStructTest") )
    {
        handled = true;
	QMap<QString, QVariant> years,months,days,day;
	years = params[0].toMap();
	months = years["2000"].toMap();
	days = months["04"].toMap();
	day = days["01"].toMap();

	int result = 0;
	result += day["larry"].toInt();
	result += day["moe"].toInt();
	result += day["curly"].toInt();
        	
        sendMethodResponse(req, QVariant( result ) );
    }	
}

void XRValidationServer::simpleStructReturnTest(
                    int req,
		    const QString& methodname,
                    const QVariantList& params,
                    bool& handled)
{
    if( (handled == false)
        && (methodname == "validator1.simpleStructReturnTest") )
    {
        handled = true;
        QMap<QString, QVariant> map;
	map["times10"] = params[0].toInt() * 10;
	map["times100"] = params[0].toInt() * 100;
	map["times1000"] = params[0].toInt() * 1000;
	
        sendMethodResponse(req, QVariant( map ) );
    }
}

void XRValidationServer::sumAndDifference(
		    int req,
                    const QString& methodname,
                    const QVariantList& params,
                    bool& handled)

{
    if( (handled == false)
        && (methodname == "sample.sumAndDifference") )
    {
        handled = true;
	int a,b;
	a = params[0].toInt();
	b = params[1].toInt();

	QMap<QString, QVariant> result;
	result["difference"] = a - b;
	result["sum"] = a+b;
	
        sendMethodResponse(req, QVariant( result ) );
    }
}
