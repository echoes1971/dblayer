#include "clientimpl.h"

ClientImpl::ClientImpl(QObject *parent) : QObject(parent) {
  _pclient = new XRClient(QUrl(lineEdit1->text()),this, "_pclient");

  //Connect up the slots!
  
  //We have to handle the responses from the server
  QObject::connect(_pclient, SIGNAL( methodResponse(int,const QVariant&) ),
		   this, SLOT(responseSlot(int, const QVariant&) ));
  QObject::connect(_pclient, SIGNAL( fault(int,int,const QString&) ),
		   this, SLOT(faultSlot(int,int,const QString&) ));
  QObject::connect(tabWidget2, SIGNAL(currentChanged(QWidget*)),
		   this, SLOT(setFunction(QWidget*)) ); 

  //For debugging purposes
  //_pclient->acceptCompressed(false);
  //There are 8 steps to the validation
  progressBar1->setTotalSteps(8);
  _x = 0;
  _y = 0;
  _current_widget = tab;
}

ClientImpl::~ClientImpl()
{

}

void ClientImpl::callSumDiff()
{
  QValueList<QVariant> params;
  QString method = "sample.sumAndDifference";
  params.push_back(_x);
  params.push_back(_y);
  int req = _pclient->call(method,params);
  _req_to_method.insert(req,method);
}

void ClientImpl::callValidate()
{
  /* fixup the output display */
  progressBar1->setProgress(0);
  textEdit1->clear();
  textEdit2->clear();
	
  int req;
  _validation_step = 0;
  QValueList<QVariant> params;
  QString method;
  
  //validator1.arrayOfStructsTest
  method = "validator1.arrayOfStructsTest";
  params.clear();
  QMap<QString,QVariant> tmp_map;
  QValueList<QVariant> tmp_list;
  tmp_map["larry"] = 2;
  tmp_map["curly"] = 3;
  tmp_map["moe"] = 5;
  tmp_list.push_back(tmp_map);
  tmp_map["larry"] = 7;
  tmp_map["curly"] = 11;
  tmp_map["moe"] = 13;
  tmp_list.push_back(tmp_map);
  tmp_map["larry"] = 17;
  tmp_map["curly"] = 19;
  tmp_map["moe"] = 23;
  tmp_list.push_back(tmp_map);
  params.push_back(tmp_list);
  req = _pclient->call(method,params);
  _req_result.insert(req,(3+11+19));
  _req_to_method.insert(req,method);
  //validator1.countTheEntities
  method = "validator1.countTheEntities";
  params.clear();
  params.push_back("<<<>>&\'\'\"\"\"");
  req = _pclient->call(method,params);
  tmp_map.clear();
  tmp_map["ctLeftAngleBrackets"] = 3;
  tmp_map["ctRightAngleBrackets"] = 2;
  tmp_map["ctAmpersands"] = 1;
  tmp_map["ctApostrophes"] = 2;
  tmp_map["ctQuotes"] = 3;
  _req_result.insert(req,tmp_map);
  _req_to_method.insert(req,method);

  //validator1.easyStructTest
  method = "validator1.easyStructTest";
  params.clear();
  tmp_map.clear();
  tmp_map["larry"] = 17;
  tmp_map["curly"] = 19;
  tmp_map["moe"] = 23;
  params.push_back(tmp_map);
  req = _pclient->call(method,params);
  _req_result.insert(req,(17+19+23));
  _req_to_method.insert(req,method);

  //validator1.echoStructTest
  method = "validator1.echoStructTest";
  params.clear();
  params.push_back(tmp_map);
  req = _pclient->call(method,params);
  _req_result.insert(req,tmp_map);
  _req_to_method.insert(req,method);

  //validator1.manyTypesTest
  method = "validator1.manyTypesTest";
  params.clear();
  params.push_back(6);
  //QVariant needs to have its hand help for bools
  params.push_back(QVariant(false,0));
  params.push_back(QString("test string"));
  params.push_back(0.123456789);
  params.push_back(QDateTime::currentDateTime(Qt::UTC));
  params.push_back(QByteArray(QCString("This is a test of byte array."
			      "Hopefully this should be base64 encoded"
			      "blah blah blah blah blah blah blah blah blah")));
  req = _pclient->call(method,params);
  _req_result.insert(req,params);
  _req_to_method.insert(req,method);

  //validator1.moderateSizeArrayCheck
  method = "validator1.moderateSizeArrayCheck";
  params.clear();
  int i=187;
  tmp_list.clear();
  tmp_list.push_back("This is the start");
  for(i = 0; i < 187; i++) {
    tmp_list.push_back("blah blah blah blah blah blah blah blah blah");
  }
  tmp_list.push_back(", and this is the end");
  params.push_back(tmp_list);
  req = _pclient->call(method,params);
  _req_result.insert(req,QString("This is the start, and this is the end"));
  _req_to_method.insert(req,method);
  
  //validator1.nestedStructTest
  method = "validator1.nestedStructTest";
  params.clear();
  tmp_map.clear();
  QMap<QString,QVariant> map2;
  map2["larry"] = 100;
  map2["curly"] = 200;
  map2["moe"] = 400;
  tmp_map["01"] = map2;
  map2.clear();
  map2["04"] = tmp_map;
  tmp_map.clear();
  tmp_map["2000"] = map2;
  map2["larry"] = 12314;
  map2["curly"] = 3842134;
  map2["moe"] = 343;
  tmp_map["junk"] = map2;
  params.push_back(tmp_map);
  req = _pclient->call(method,params);
  _req_result.insert(req,700);
  _req_to_method.insert(req,method);
  
  //validator1.simpleStructReturnTest
  method = "validator1.simpleStructReturnTest";
  params.clear();
  params.push_back(42);
  req = _pclient->call(method,params);
  tmp_map.clear();
  tmp_map["times10"] = 420;
  tmp_map["times100"] = 4200;
  tmp_map["times1000"] = 42000;
  _req_result.insert(req,tmp_map);
  _req_to_method.insert(req,method);
}

void ClientImpl::faultSlot(int req, int faultNum, const QString& s)
{
  QMessageBox::warning(0,
		       "Server Fault",
		       "Fault Number: " + QString::number(faultNum) + "\n"
		       + "Fault String: " + s,
		       QMessageBox::Ok,QMessageBox::NoButton);
  _req_to_method.erase(req);
}

void ClientImpl::setFunction(QWidget* w)
{
  //Disconnect all the call slots
  disconnect(pushButton1, SIGNAL( clicked()), this, SLOT( callSumDiff() ));
  disconnect(pushButton1, SIGNAL( clicked()), this, SLOT( callValidate() ));
  if(w == tab) {
    connect(pushButton1, SIGNAL( clicked() ),
	    this, SLOT( callSumDiff() ) );
  }
  else if( w == tab_2 ) {
    connect(pushButton1, SIGNAL( clicked() ),
	    this, SLOT( callValidate() ) );

  }
  _current_widget = w;
}

void ClientImpl::setX(const QString& x)
{
  _x = x.toInt();
  lineEdit2->setText( QString::number(_x) );
}

void ClientImpl::setY(const QString& y)
{
 _y = y.toInt();
 lineEdit3->setText( QString::number(_y) );
}

void ClientImpl::responseSlot(int req, const QVariant& v)
{

  //Get the widget that this is a response for.
  QString s = _req_to_method[req];
  if( s == "sample.sumAndDifference" ) {
    //This is a sample sumAndDifference response
    QMapConstIterator<QString, QVariant> it;
    it = v.mapFind("sum");
    lCDNumber1->display( it.data().toInt() );
    
    it = v.mapFind("difference");
    lCDNumber2->display( it.data().toInt() );
  }
  else if( s == "validator1.arrayOfStructsTest") {
    progressBar1->setProgress( ++_validation_step );
    if( v == _req_result[req] ) {
        textEdit1->append(s + "\n");   
    }
    else {
        textEdit2->append(s + "\n");   
    }
    _req_result.erase(req);
  }
  else if( s == "validator1.countTheEntities") {
    progressBar1->setProgress( ++_validation_step );
    QMapConstIterator<QString, QVariant> it = v.mapBegin();
    bool eq = true;
    QMap<QString,QVariant> tmp_map = _req_result[req].toMap();
    while((it != v.mapEnd()) && eq) {
      eq = eq && (tmp_map[it.key()] == it.data());
      it++;
    }
    if( eq ) {
        textEdit1->append(s + "\n");   
    }
    else {
        textEdit2->append(s + "\n");   
    }
    _req_result.erase(req);

  }
  else if( s == "validator1.easyStructTest") {
    progressBar1->setProgress( ++_validation_step );
    if( v == _req_result[req] ) {
        textEdit1->append(s + "\n");   
    }
    else {
        textEdit2->append(s + "\n");   
    }
    _req_result.erase(req);

  }
  else if( s == "validator1.echoStructTest") {
    progressBar1->setProgress( ++_validation_step );
    QMapConstIterator<QString, QVariant> it = v.mapBegin();
    bool eq = true;
    QMap<QString,QVariant> tmp_map = _req_result[req].toMap();
    while((it != v.mapEnd()) && eq) {
      eq = eq && (tmp_map[it.key()] == it.data());
      it++;
    }
    if( eq ) {
        textEdit1->append(s + "\n");   
    }
    else {
        textEdit2->append(s + "\n");   
    }
    _req_result.erase(req);

  }
  else if( s == "validator1.manyTypesTest") {
    progressBar1->setProgress( ++_validation_step );
    QValueListConstIterator<QVariant> it = v.listBegin();
    QValueListConstIterator<QVariant> it2 = _req_result[req].listBegin();
    bool eq = true;
    while((it != v.listEnd()) && eq) {
      //This will work for all the types XML-RPC can support
      //except maps and lists, but we don't expect those
      eq = eq && ((*it).type() == (*it2).type());
      eq = eq && ((*it2).toString() == (*it).toString());
      it2++;
      it++;
    }
    if( eq ) {
        textEdit1->append(s + "\n");   
    }
    else {
        textEdit2->append(s + "\n");   
    }
    _req_result.erase(req);

  }
  else if( s == "validator1.moderateSizeArrayCheck") {
    progressBar1->setProgress( ++_validation_step );
    if( v == _req_result[req] ) {
        textEdit1->append(s + "\n");   
    }
    else {
        textEdit2->append(s + "\n");   
    }
    _req_result.erase(req);

  }
  else if( s == "validator1.nestedStructTest") {
    progressBar1->setProgress( ++_validation_step );
    if( v == _req_result[req] ) {
        textEdit1->append(s + "\n");   
    }
    else {
        textEdit2->append(s + "\n");   
    }
    _req_result.erase(req);

  }
  else if( s == "validator1.simpleStructReturnTest") {
    progressBar1->setProgress( ++_validation_step );
    QMapConstIterator<QString, QVariant> it = v.mapBegin();
    bool eq = true;
    QMap<QString,QVariant> tmp_map = _req_result[req].toMap();
    while((it != v.mapEnd()) && eq) {
      eq = eq && (tmp_map[it.key()] == it.data());
      it++;
    }
    if( eq ) {
        textEdit1->append(s + "\n");   
    }
    else {
        textEdit2->append(s + "\n");   
    }
    _req_result.erase(req);

  }
  //Forget about this request.
  _req_to_method.erase(req);
}
