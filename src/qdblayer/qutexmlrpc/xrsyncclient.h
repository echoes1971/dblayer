#ifndef XRSYNCCLIENT_H
#define XRSYNCCLIENT_H

#include <QObject>
#include <QThread>

#include <xrclient.h>

class XRSyncClient : public QObject {
    Q_OBJECT

  public:
    /**
     * @param server_url the URL to the server for this client
     */
    XRSyncClient(const QUrl& server_url, QObject * parent = 0);
    ~XRSyncClient();


    QVariant* call(const QString& method, const QList<QVariant>& params);

    void acceptCompressed(bool b);

  private:
    XRClient* _client;
    QUrl server_url;
    QObject* parent;
};

class XRSyncClientThread : public QThread {
    Q_OBJECT
  public:
    void run();

    QUrl server_url;
    QObject* parent;
    XRClient* _client;
    QString method;
    QList<QVariant> params;

    bool hasResponse;
    bool hasFault;

    int faultNum;
    QString faultString;
    int http_req;

    QVariant resp;

    QMutex mutex;
    QWaitCondition qwc;

  public slots:
    // XmlRpc
    void responseSlot(int num, const QVariant& v);
    void faultSlot(int num, int fault_num, const QString& string);
};

#endif // XRSYNCCLIENT_H
