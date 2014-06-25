#include "qutexmlrpc/xrsyncclient.h"

XRSyncClient::XRSyncClient(const QUrl& server_url, QObject * parent) {
    this->server_url = server_url;
    this->parent = parent;
    this->_client = 0; //new XRClient(server_url, parent);

}
XRSyncClient::~XRSyncClient() {
    delete this->_client;
}

QVariant* XRSyncClient::call(const QString& method, const QList<QVariant>& params) {
    printf("XRSyncClient::call: threadid=%x\n", this->thread()->currentThreadId() );
    QVariant* ret = 0;

    XRSyncClientThread t;
    t._client = 0; //new XRClient(server_url, parent);;
    t.method = method;
    t.params = params;
    t.server_url = this->server_url;
    t.parent = this->parent;

    t.start();
    /*
    t.mutex.lock();
    bool hasDone = t.qwc.wait(&(t.mutex),10000);
    printf("XRSyncClient::call: hasDone=%s\n", (hasDone?"true":"false") );
    t.mutex.unlock();
    */
    bool hasDone = t.wait(5000);
    //if(hasDone)
        ret = new QVariant(t.resp);

    delete t._client;

    return ret;
}


void XRSyncClient::acceptCompressed(bool b) {
    this->_client->acceptCompressed(b);
}






// ****************** XRSyncClientThread

void XRSyncClientThread::run() {
    printf("XRSyncClientThread::run: threadid=%x\n", this->thread()->currentThreadId() );
    this->hasResponse = false;
    this->hasFault = false;

    this->_client = new XRClient(server_url); //, this);

    //We have to handle the responses from the server
    printf("XRSyncClientThread::run: Connectiong signals...\n");
    QObject::connect(this->_client, SIGNAL( methodResponse(int,const QVariant&) ),
                     this, SLOT(responseSlot(int, const QVariant&) ), Qt::QueuedConnection);
    QObject::connect(this->_client, SIGNAL( fault(int,int,const QString&) ),
                     this, SLOT(faultSlot(int,int,const QString&) ), Qt::QueuedConnection); //Qt::DirectConnection);


    printf("XRSyncClientThread::run: Calling remote method...\n");
    http_req = this->_client->call(method, params);
    printf("XRSyncClientThread::run: http_req=%d\n",http_req);

    printf("XRSyncClientThread::run: this->hasResponse=%d this->hasFault=%d\n",this->hasResponse,this->hasFault);
    /**/
    while(!this->hasResponse && !this->hasFault) {
        this->msleep(500);
        printf("XRSyncClientThread::run: after sleep this->hasResponse=%s this->hasFault=%s\n",
               (this->hasResponse?"true":"false"),
               (this->hasFault?"true":"false")
               );
    }
    /**/
}

void XRSyncClientThread::faultSlot(int num, int faultNum, const QString& s) {
    printf("XRSyncClientThread::faultSlot: threadid=%x\n", this->thread()->currentThreadId() );
    printf("XRSyncClientThread::faultSlot: num=%d, http_req=%d\n",num, http_req);
    if(http_req!=num)
        return;

    this->faultNum=faultNum;
    this->faultString=s;

    this->hasFault = true;

    this->mutex.lock();
    this->qwc.wakeAll();
    this->mutex.unlock();
}
void XRSyncClientThread::responseSlot(int num, const QVariant& v) {
    printf("XRSyncClientThread::responseSlot: threadid=%x\n", this->thread()->currentThreadId() );
    printf("XRSyncClientThread::responseSlot: num=%d, http_req=%d\n",num, http_req);
    if(http_req!=num)
        return;

    this->resp = v;
    this->hasResponse = true;

    this->mutex.lock();
    this->qwc.wakeAll();
    this->mutex.unlock();
}


