#ifndef CLIENTIMPL_H
#define CLIENTIMPL_H
#include "../xrclient.h"
#include <qmap.h>

class ClientImpl : public QObject
{
    Q_OBJECT

public:
    ClientImpl(QObject * parent = 0);
    ~ClientImpl();

protected slots:
    void callSumDiff();
    void callValidate();
    /**
     * connected to the QTabWidget's currentChanged signal
     */
    void setFunction(QWidget* wid);

    /**
     * Sets the x and y values for the sumAndDifference method
     */
    void setX(const QString& x);
    void setY(const QString& y);

    void responseSlot(int num, const QVariant& v);
    void faultSlot(int num, int fault_num, const QString& string);

protected:
    int _x,_y;
    XRClient* _pclient;
    /**
     * Holds the active tab, so we know what do when the call
     * slot is active.
     */
    QWidget* _current_widget;
    /**
     * This map keeps track of qhich requests are for what functions.
     */
    QMap<int, QString> _req_to_method;

    /**
     * For validation client.  Holds the result we are expecting
     */
    QMap<int, QVariant> _req_result;
    int _validation_step;

};

#endif // CLIENTIMPL_H
