#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QMap>
#include <QtCore/QThread>
#include <QtGui/QMainWindow>

#include <dblayer/dbconnection.h>
#include <dblayer/dbefactory.h>


namespace Ui {
    class MainWindow;
}

class MyThread;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void clearText();
    void appendText(QString s);
    void writeLine(QString s="");

    void printFieldCreatiDistrutti();

    bool connect(QString connString);

    void testDateField();
    void testLogin(QString connString, QString user, QString pwd, Connection* con=0);
    void testSchemaName(QString connString, Connection* con=0);
    void testDBConnection(QString connString, QString query="select * from rra_users order by id desc", Connection* con=0);
    void testGetKeys(QString connString, QString relname, Connection* con=0);
    void testGetForeignKeys(QString connString, QString relname, Connection* con=0);
    void testDBMgr(QString connString, QString aQuery="select * from test_dblayer", QString aNomeTabella="test_dblayer", Connection* con=0);
    void testDBE(QString connString, Connection* con=0);
    void testCRUD(QString connString, Connection* con=0);
    void testGetFormSchema(QString connString, QString language="python", Connection* con=0);
    void testGetDBSchema(QString connString, QString language="python", Connection* con=0);

    void runMyThread(QString connString, QString connUser="adm", QString connPwd="adm");

    DBEFactory* dbeFactory;
public slots:
    void slotTest();
    void slotTestSchema();
    void slotTestThreads();

    void slotConnected(bool b);
    void slotResultSetReady(int req);
    void slotResultSetError(int req,QString errorMsg);

    void slotThreadStarted();
    void slotThreadCompleted();

    void slotAppendText(QString s);
    void slotClearText();
signals:
    void signalAppendText(QString);
    void signalClearText();

private:

    Ui::MainWindow *ui;

    QString connString;
    DBLayer::Connection* con;
    QMap<int,DBLayer::ResultSet*> res;

    QMap<QString,MyThread*> mythreads;
    int threadsRunning;
};

class MyThread : public QThread {
  public:
    QString nomeThread;
    QString connString;
    QString connUser;
    QString connPassword;
    MainWindow* c;
    void (MainWindow::*myjob)(QString,QString,QString);
    void run();
};

#endif // MAINWINDOW_H
