#include <cstdio>
#include <typeinfo>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dblayer/dbconnection.h"
#include "dblayer/dbmgr.h"
#include "dblayer/datefield.h"
#include "qdblayer.h"
#include "qxmlrpcconnection.h"
using namespace DBLayer;

#include "dbschema.h"
using namespace MySchema;

// ********** Class MyThread: start.
void MyThread::run() {
    printf("%0lx::MyThread::run: start %s.\n", (unsigned long) QThread::currentThread(), this->nomeThread.toStdString().c_str());
    (c->*myjob)(this->connString,this->connUser,this->connPassword);
    printf("%0lx::MyThread::run: end %s.\n", (unsigned long) QThread::currentThread(), this->nomeThread.toStdString().c_str());
}
// ********** Class MyThread: end.


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

#ifdef __APPLE__
    this->ui->comboBox->addItem("dblayer:qxmlrpc:http://localhost/~robertoroccoangeloni/rproject/xmlrpc_server.php");
#else
    this->ui->comboBox->addItem("dblayer:qxmlrpc:http://localhost/~roberto/rproject/xmlrpc_server.php");
#endif
    this->ui->comboBox->addItem("dblayer:qxmlrpc:http://www.roccoangeloni.it/rproject/xmlrpc_server.php");
    this->ui->comboBox->addItem("dblayer:qxmlrpc:http://www.kware.it/xmlrpc_server.php");
#ifdef __APPLE__
    this->ui->comboBox->addItem("dblayer:qxmlrpc:http://localhost/~robertoroccoangeloni/xmlrpc/server.php");
#else
    this->ui->comboBox->addItem("dblayer:qxmlrpc:http://localhost/~roberto/xmlrpc/server.php");
#endif
    this->ui->comboBox->addItem("dblayer:sqlite:./test.db");
    this->ui->comboBox->addItem("dblayer:sqlite:.\\test.db");
    this->ui->comboBox->addItem("dblayer:odbc:roberto");
    this->ui->comboBox->addItem("dblayer:odbc:test_dblayer");
    this->ui->comboBox->addItem("dblayer:pg:host=localhost dbname=roberto user=roberto password=");
    this->ui->comboBox->addItem("dblayer:mysql:host=localhost;dbname=roberto;user=root;password=;");

    this->ui->comboQuery->addItem("select * from rra_users order by id desc");
    this->ui->comboQuery->addItem("select * from test_dblayer order by id desc");
    this->ui->comboQuery->addItem("select * from societa");
    this->ui->comboQuery->addItem("select oid,typname from pg_type");

    QXmlrpcConnection::registerClass();

    QObject::connect(this,SIGNAL(signalClearText()),this,SLOT(slotClearText()));
    QObject::connect(this,SIGNAL(signalAppendText(QString)),this,SLOT(slotAppendText(QString)));

    this->con=0;
    this->threadsRunning=0;
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::clearText() {
    if(QThread::currentThread()!=this->thread())
        emit signalClearText();
    else
        this->ui->textEdit->document()->clear();
}
void MainWindow::appendText(QString s) {
    if(QThread::currentThread()!=this->thread()) {
        printf("[%0lx]%s",(unsigned long)QThread::currentThread(), s.toStdString().c_str());
        //emit signalAppendText(QString("[%1]%2").arg( QString("0x%1").arg((unsigned int)QThread::currentThread()), s) );
    } else
        this->ui->textEdit->document()->setPlainText( this->ui->textEdit->document()->toPlainText().append(s) );
}
void MainWindow::writeLine(QString s) {
    this->appendText( s + "\n" );
    //printf("%s\n",s.toStdString().c_str());
}

void MainWindow::printFieldCreatiDistrutti() {
    this->writeLine( QString(
            "Field Creati: %1 - Field Distrutti: %2 - Schemi Creati: %3 - Schemi Distrutti: %4"
            ).arg(SchemaNS::getFieldCreati()).arg(SchemaNS::getFieldDistrutti()
            ).arg(SchemaNS::getSchemiCreati()).arg(SchemaNS::getSchemiDistrutti()) );
}

void MainWindow::testDateField() {
    DBLayer::DateField dateField = DBLayer::DateField( new string("cippa"),
                                    new string("1970-02-01 00:00:00") );

    this->writeLine( QString("string: ").append(dateField.toString().c_str()) );
    this->writeLine( QString("seconds: %1").arg(dateField.to_seconds()) );
    this->writeLine( QString("days: %1").arg(dateField.to_days()) );

    dateField.setValue( dateField.to_seconds() );
    this->writeLine( QString("seconds2date: ").append(dateField.toString().c_str()) );

    dateField.setValue( new string("2038-01-19 00:00:00") );
    this->writeLine( QString("date: ").append(dateField.toString().c_str()) );
    dateField.setValue( dateField.to_seconds() );
    this->writeLine( QString("seconds2date: ").append(dateField.toString().c_str()) );
}

void MainWindow::testLogin(QString connString, QString user, QString pwd, Connection* con) {
    Connection* mycon=con;
    if(con==0) {
        if(this->con!=0) this->con->disconnect();
        this->connect(connString);
        if ( this->con->hasErrors() ) {
            this->writeLine( QString("(testLogin) Errori: %1").arg(this->con->getErrorMessage().c_str()) );
            return;
        }
        mycon=this->con;
    }
    printf("MainWindow::testLogin: mycon.isConnected=%s\n", mycon->isConnected()?"true":"false");
    if(!mycon->isConnected()) mycon->connect();

    this->writeLine( QString("User: %1 - Pwd: %2").arg(user,pwd) );

    ResultSet* rs = mycon->login( user.toStdString(), pwd.toStdString() );
    this->writeLine( QString("loggedIn: %1").arg( rs==0 ? "false" : QString(rs->toString("\n").c_str()) ) );
    delete rs;
}

void MainWindow::testSchemaName(QString connString, Connection* con) {
    Connection* mycon=con;
    if(con==0) {
        if(this->con!=0) this->con->disconnect();
        this->connect(connString);
        if ( this->con->hasErrors() ) {
            this->writeLine( QString("(testSchemaName) Errori: %1").arg(this->con->getErrorMessage().c_str()) );
            return;
        }
        mycon=this->con;
    }
    printf("MainWindow::testSchemaName: mycon.isConnected=%s\n", mycon->isConnected()?"true":"false");
    if(!mycon->isConnected()) mycon->connect();

    string schemaName = mycon->getSchemaName();
    this->writeLine( "Schema name: " + QString(schemaName.c_str()) );
    //delete rs;
}


void MainWindow::testDBConnection(QString connString, QString query, Connection* con) {
    Connection* mycon=con;
    if(con==0) {
        if(this->con!=0) this->con->disconnect();
        this->connect(connString);
        if ( this->con->hasErrors() ) {
            this->writeLine( QString("(testLogin) Errori: %1").arg(this->con->getErrorMessage().c_str()) );
            return;
        }
        mycon=this->con;
    }

    if ( !mycon->hasErrors() ) {
        QXmlrpcResultSet* tmpRS = (QXmlrpcResultSet*) mycon->exec(query.toStdString());
        this->res.insert( tmpRS->getReqNumber(), tmpRS);

        if( !mycon->hasErrors() ) {
            this->writeLine( QString("res.status: %1").arg(tmpRS->getStatus().c_str()) );
            this->writeLine( QString("res.toString() = %1").arg(tmpRS->toString().c_str()) );

            int nColonne = tmpRS->getNumColumns();
            this->writeLine( QString("N. Colonne: %1").arg(nColonne ) );
            for( int i=0; i<nColonne; i++) {
                this->appendText( QString("Colonna[%1]: ").arg(i) );
                this->appendText( QString(tmpRS->getColumnName(i).c_str()) );
                this->appendText( QString(" - %1").arg(tmpRS->getColumnType(i).c_str()) );
                this->appendText( QString(": %1\n").arg(tmpRS->getColumnSize(i)) );
            }

            int nRighe = tmpRS->getNumRows();
            this->writeLine( QString(" Righe: %1").arg(nRighe) );
            for(int r=0; r<nRighe; r++) {
                for(int c=0; c<nColonne; c++) {
                    if (! tmpRS->isNull(r,c) ) {
                        this->appendText( QString("%1\t").arg(tmpRS->getValue(r,c).c_str()) );
                    } else {
                        this->appendText( QString("\\N\t") );
                    }
                }
                this->writeLine();
            }
        } else {
            this->writeLine( QString("Errori: %1").arg(mycon->getErrorMessage().c_str()) );
        }
        delete tmpRS;
    } else {
        this->writeLine( QString("(testDBConnection) Errori: %1").arg(mycon->getErrorMessage().c_str()) );
    }
}
void MainWindow::testGetKeys(QString connString, QString relname, Connection* con) {
    Connection* mycon=con;
    if(con==0) {
        if(this->con!=0) this->con->disconnect();
        this->connect(connString);
        if ( this->con->hasErrors() ) {
            this->writeLine( QString("(testLogin) Errori: %1").arg(this->con->getErrorMessage().c_str()) );
            return;
        }
        mycon=this->con;
    }

    if ( !mycon->hasErrors() ) {
        this->writeLine( QString("Tabella: %1").arg(relname) );
        string s_relname = relname.toStdString();

        int numColonne = mycon->getColumnSize( &s_relname );
        this->writeLine( QString("\tNum. Colonne: %1").arg(numColonne) );
        for(int c = 1; c<=numColonne; c++) {
            this->appendText( QString("\t%1").arg(c) );
            this->writeLine( QString(") %1").arg( mycon->getColumnName( &s_relname, c ).c_str() ) );
        }

        DBLayer::IntegerVector chiavi = mycon->getKeys(&s_relname);

        if( !mycon->hasErrors() ) {
            unsigned int nChiavi = (unsigned int) chiavi.size();
            for(unsigned int i=0; i<nChiavi; i++) {
                this->appendText( QString("Colonna chiave:\t%1").arg(chiavi[i]) );
                this->writeLine( QString("\t%1").arg( mycon->getColumnName( &s_relname, chiavi[i] ).c_str() ) );
            }
        } else {
            this->writeLine( QString("(testDBConnection) Errori: %1").arg(mycon->getErrorMessage().c_str()) );
        }
    } else {
        this->writeLine( QString("(testDBConnection) Errori: %1").arg(mycon->getErrorMessage().c_str()) );
    }
}
void MainWindow::testGetForeignKeys(QString connString, QString relname, Connection* con) {
    Connection* mycon=con;
    if(con==0) {
        if(this->con!=0) this->con->disconnect();
        this->connect(connString);
        if ( this->con->hasErrors() ) {
            this->writeLine( QString("(testLogin) Errori: %1").arg(this->con->getErrorMessage().c_str()) );
            return;
        }
        mycon=this->con;
    }
    if ( !mycon->hasErrors() ) {
        string s_relname = relname.toStdString();
        this->writeLine( QString("Tabella: %1").arg(relname) );
        int numColonne = mycon->getColumnSize( &s_relname );
        this->writeLine( QString("\tNum. Colonne: %1").arg(numColonne) );
        for(int c = 1; c<=numColonne; c++) {
            this->appendText( QString("\t%1").arg(c) );
            this->writeLine( QString(") %1").arg( mycon->getColumnName( &s_relname, c ).c_str() ) );
        }
        DBLayer::IntegerVector chiavi = mycon->getForeignKeys(&s_relname);
        if( !mycon->hasErrors() ) {
            unsigned int nChiavi = (unsigned int) chiavi.size();
            for(unsigned int i=0; i<nChiavi; i++) {
                this->appendText( QString("Colonna chiave:\t%1").arg(chiavi[i]) );
                this->writeLine( QString("\t%1").arg( mycon->getColumnName( &s_relname, chiavi[i] ).c_str() ) );
            }
        } else {
            this->writeLine( QString("(testGetForeignKeys) Errori: %1").arg(mycon->getErrorMessage().c_str()) );
        }
    } else {
        this->writeLine( QString("(testGetForeignKeys) Errori: %1").arg(mycon->getErrorMessage().c_str()) );
    }
}
void MainWindow::testDBMgr(QString connString, QString aQuery, QString aNomeTabella, Connection* con) {
    Connection* mycon=con;
    if(con==0) {
        if(this->con!=0) this->con->disconnect();
        this->connect(connString);
        if ( this->con->hasErrors() ) {
            this->writeLine( QString("(testLogin) Errori: %1").arg(this->con->getErrorMessage().c_str()) );
            return;
        }
        mycon=this->con;
    }
    DBMgr* dbmgr;

    dbmgr = new DBLayer::DBMgr(mycon, false);
    dbmgr->setDBEFactory(this->dbeFactory);

    if ( dbmgr->connect() ) {
        //string nomeTabella = aNomeTabella.toStdString();
	string nomeTabella = aNomeTabella.replace("rra_","").toStdString();
        string myQuery = aQuery.toStdString();
        DBEntityVector* lista = dbmgr->Select( &nomeTabella, &myQuery );

        if ( lista!=0 && lista->size()>0 ) {
            this->appendText( QString("Lista (%1) di ").arg(typeid(lista).name()) );
            this->writeLine( QString(" elementi: %1").arg(lista->size()) );
            DBEntityVector::iterator theIterator;
            for(theIterator = lista->begin(); theIterator!=lista->end(); theIterator++) {
                string miastringa = (*theIterator)->toString();
                this->writeLine( QString("\t%1").arg(miastringa.c_str()) );
            }
            this->writeLine( QString("====") );
        } else {
            this->writeLine( QString("testDBMgr: LISTA VUOTA!!!") );
        }

        this->writeLine( QString("testDBMgr: destroying lista...") );
        DBMgr::Destroy(lista);
// 		dbmgr->Destroy(lista);
        this->writeLine( QString("OK!") );

        this->writeLine( QString("testDBMgr: address of content lista after destroy = %1").arg((long) lista) );
    } else {
        this->writeLine( QString("(testDBMgr) Errore: %1").arg(dbmgr->getErrorMessage().c_str()) );
    }
    delete dbmgr;
}
void MainWindow::testDBE(QString connString, Connection* con) {
    Connection* mycon=con;
    if(con==0) {
        if(this->con!=0) this->con->disconnect();
        this->connect(connString);
        if ( this->con->hasErrors() ) {
            this->writeLine( QString("(testLogin) Errori: %1").arg(this->con->getErrorMessage().c_str()) );
            return;
        }
        mycon=this->con;
    }
    DBMgr* dbmgr;
    DBEFactory* dbeFactory;

    dbmgr = new DBLayer::DBMgr(mycon, false);

    dbeFactory = new DBEFactory();
    dbeFactory->registerClass(string("societa"),new DBESocieta());
    //dbeFactory->registerClass(string("test_dblayer"),new DBETestDBLayer());

    dbmgr->setDBEFactory(dbeFactory);

    // Stampo la factory
    this->writeLine( QString("testDBE: factory = %1").arg(dbeFactory->toString().c_str()) );

    // Stampo le classi registrate
    DBEntityVector myTypes = dbmgr->getRegisteredTypes();
    for(unsigned int i=0; i<myTypes.size(); i++) {
        this->appendText( QString("testDBE: registered type %1").arg(i) );
        this->appendText( QString(" - %1").arg((long)myTypes[i]) );
        this->writeLine( QString(" - %1").arg(myTypes[i]->toString().c_str()) );
    }

    if ( dbmgr->connect() ) {
        string myclazzname("societa");
        string mytypename("DBESocieta");
        DBEntity* cerca = dbmgr->getClazzByTypeName(&mytypename);

        string nomeCampo("ragione_sociale");
        string valoreStringa("a");
        cerca->setValue( &nomeCampo, &valoreStringa );

//		string nomeCampoInteger("cap");
//		long valoreInteger = 20156;
//		cerca->setValue( &nomeCampoInteger, valoreInteger );

        this->writeLine( QString("testDBE: cerca = %1").arg(cerca->toString().c_str()) );

        this->appendText( QString("testDBE: cerca.getNames() = { ") );
        DBLayer::StringVector nomi = cerca->getNames();
        for(unsigned int i=0; i<nomi.size(); i++) {
            this->appendText( QString("%1").arg(nomi[i].c_str()) );
            if( i!=(nomi.size()-1) )
                this->appendText( QString(", ") );
        }
        this->writeLine( QString(" }") );

        string orderBy = string("ragione_sociale");
        DBEntityVector* lista = dbmgr->Search(cerca, true, true, &orderBy );
        this->writeLine( QString("testDBE: ricerca completata.") );

        if ( lista!=0 && lista->size()>0 ) {
            this->writeLine( QString("Lista (%1):").arg( DBLayer::integer2string((long)lista->size()).c_str()) );
            DBEntityVector::iterator theIterator;
            for(theIterator = lista->begin(); theIterator!=lista->end(); theIterator++) {
                this->writeLine( QString("\t%1").arg((*theIterator)->toString().c_str()) );
            }
        }

        delete cerca;
        dbmgr->Destroy(lista);
    } else {
        this->writeLine( QString("Errore: %1").arg(dbmgr->getErrorMessage().c_str()) );
    }
    delete dbmgr;
}
void MainWindow::testCRUD(QString connString, Connection* con) {
    Connection* mycon=con;
    if(con==0) {
        if(this->con!=0) this->con->disconnect();
        this->connect(connString);
        if ( this->con->hasErrors() ) {
            this->writeLine( QString("(testLogin) Errori: %1").arg(this->con->getErrorMessage().c_str()) );
            return;
        }
        mycon=this->con;
    }
    if(!mycon->isConnected()) mycon->connect();

    DBMgr* dbmgr;
//    DBEFactory* dbeFactory;

    dbmgr = new DBLayer::DBMgr(mycon, true);

    dbmgr->setDBEFactory(this->dbeFactory);

    this->writeLine( QString("testCRUD: Creata DBEFactory.") );

    if ( dbmgr->connect() ) {
        string myclazzname("societa");
        DBEntity* nuova = dbmgr->getClazz(&myclazzname);
        string nomeCampo("ragione_sociale");
        string valoreStringa("Nuova Societa S.r.l.");
        nuova->setValue( &nomeCampo, &valoreStringa );
        nomeCampo="data_creazione";
        valoreStringa="1970-02-01 00:00:00";
        nuova->setDateValue(&nomeCampo,&valoreStringa);

        this->writeLine( QString("testCRUD: da inserire %1").arg(nuova->toString().c_str()) );

        // Insert
        this->writeLine();
        this->writeLine( QString("testCRUD: provo ad inserire una nuova dbe ") );
        nuova = dbmgr->Insert(nuova);
        this->writeLine( QString("testCRUD: inserita nuova dbe %1").arg(nuova->toString().c_str()) );

        DBEntity* cerca = dbmgr->getClazz(&myclazzname);
        string nomeCampoId("id");
        DBField* idField = (DBField*) nuova->getField(&nomeCampoId);
        cerca->setValue(&nomeCampoId, idField!=0 ? idField->getIntegerValue() : -1 );
        string orderBy = string("id");
        this->writeLine( QString("testCRUD: cerca=%1").arg(cerca->toString().c_str()) );

        this->writeLine();
        DBEntityVector* lista = dbmgr->Search(cerca, false, true, &orderBy );
        if ( lista!=0 && lista->size()>0 ) {
            this->writeLine( QString("Lista (%1):").arg(DBLayer::integer2string((long)lista->size()).c_str()) );
            for(DBEntityVector::iterator theIterator = lista->begin(); theIterator!=lista->end(); theIterator++) {
                this->writeLine( QString("- %1").arg((*theIterator)->toString().c_str()) );
            }
        } else {
            this->writeLine( QString("testCRUD: LISTA VUOTA!!!") );
        }
        dbmgr->Destroy(lista);

        // Update
        string valoreStringa2("Nuova Societa L\'Attico S.r.l.");
        nuova->setValue( &nomeCampo, &valoreStringa2 );
        string nomeCampoCap("cap");
        nuova->setValue( &nomeCampoCap, 60015L );

        this->writeLine();
        nuova = dbmgr->Update(nuova);
        this->writeLine( QString("testCRUD: modificata nuova dbe %1").arg(nuova->toString().c_str()) );

        this->writeLine();
        lista = dbmgr->Search(cerca, true, true, &orderBy );
        if ( lista!=0 && lista->size()>0 ) {
            this->writeLine( QString("Lista (%1):").arg(DBLayer::integer2string((long)lista->size()).c_str()) );
            for(DBEntityVector::iterator theIterator = lista->begin(); theIterator!=lista->end(); theIterator++) {
                this->writeLine( QString("- %1").arg((*theIterator)->toString().c_str()) );
            }
        } else {
            this->writeLine( QString("testCRUD: LISTA VUOTA!!!") );
        }
        dbmgr->Destroy(lista);

        // Copy
        this->writeLine();
        DBEntity* copia = dbmgr->Copy( nuova );
        if(copia!=0) {
            this->writeLine( QString("testCRUD: creata copia dbe %1").arg(copia->toString().c_str()) );
        } else {
            this->writeLine( QString("testCRUD: FALLITA copia") );
        }

        this->writeLine();
        DBEntity* cercaCopia = dbmgr->getClazz(&myclazzname);
        string nomeCampoRagioneSociale("ragione_sociale");
        string valoreCampoRagioneSociale("Societa");
        cercaCopia->setValue( &nomeCampoRagioneSociale, &valoreCampoRagioneSociale );
        lista = dbmgr->Search(cercaCopia, true, true, &orderBy );
        if ( lista!=0 && lista->size()>0 ) {
            this->writeLine( QString("Lista (%1):").arg(DBLayer::integer2string((long)lista->size()).c_str()) );
            for(DBEntityVector::iterator theIterator = lista->begin(); theIterator!=lista->end(); theIterator++) {
                this->writeLine( QString("- %1").arg((*theIterator)->toString().c_str()) );
            }
        } else {
            this->writeLine( QString("testCRUD: LISTA VUOTA!!!") );
        }
        dbmgr->Destroy(lista);


        // Delete
        this->writeLine();
        nuova = dbmgr->Delete(nuova);
        this->writeLine( QString("testCRUD: cancellata nuova dbe %1").arg(nuova->toString().c_str()) );
        this->writeLine();
        if(copia!=0) {
            copia = dbmgr->Delete(copia);
            this->writeLine( QString("testCRUD: cancellata copia dbe %1").arg(copia->toString().c_str()) );
        }

        this->writeLine();
        lista = dbmgr->Search(cercaCopia, true, true, &orderBy );
        if ( lista!=0 && lista->size()>0 ) {
            this->writeLine( QString("Lista (%1):").arg(DBLayer::integer2string((long)lista->size()).c_str()) );
            for(DBEntityVector::iterator theIterator = lista->begin(); theIterator!=lista->end(); theIterator++) {
                this->writeLine( QString("- %1").arg((*theIterator)->toString().c_str()) );
            }
        } else {
            this->writeLine( QString("testCRUD: LISTA VUOTA!!!") );
        }
        dbmgr->Destroy(lista);

        delete nuova;
        if(copia!=0) delete copia;
        delete cerca;
        delete cercaCopia;
    } else {
        this->writeLine( QString("testCRUD: ERRORE %1").arg(dbmgr->getErrorMessage().c_str()) );
    }
    delete dbmgr;
}
void MainWindow::testGetFormSchema(QString connString, QString language, Connection* con) {
    Connection* mycon=con;
    if(con==0) {
        if(this->con!=0) this->con->disconnect();
        this->connect(connString);
        if ( this->con->hasErrors() ) {
            this->writeLine( QString("(testLogin) Errori: %1").arg(this->con->getErrorMessage().c_str()) );
            return;
        }
        mycon=this->con;
    }

    if ( !mycon->hasErrors() ) {
        QString schema( mycon->getFormSchema( language.toStdString() ).c_str() );
        this->writeLine( "Form Schema\n" );
        this->writeLine( schema );
    } else {
        this->writeLine( QString("(testGetFormSchema) Errori: %1").arg(this->con->getErrorMessage().c_str()) );
    }
}
void MainWindow::testGetDBSchema(QString connString, QString language, Connection* con) {
    Connection* mycon=con;
    if(con==0) {
        if(this->con!=0) this->con->disconnect();
        this->connect(connString);
        if ( this->con->hasErrors() ) {
            this->writeLine( QString("(testLogin) Errori: %1").arg(this->con->getErrorMessage().c_str()) );
            return;
        }
        mycon=this->con;
    }

    if ( !mycon->hasErrors() ) {
        QString schema( mycon->getDBSchema( language.toStdString() ).c_str() );
        this->writeLine( "DB Schema\n" );
        this->writeLine( schema );
    } else {
        this->writeLine( QString("(testGetDBSchema) Errori: %1").arg(mycon->getErrorMessage().c_str()) );
    }
}

bool MainWindow::connect(QString connString) {
    printf("MainWindow::connect: this->connString==connString => %s\n", this->connString==connString?"true":"false");
    printf("MainWindow::connect: this->con!=0 => %s\n", this->con!=0?"true":"false");
    if(this->con!=0) printf("MainWindow::connect: this->con->isConnected() => %s\n", this->con->isConnected()?"true":"false");
    if(this->connString==connString && this->con!=0 && this->con->isConnected())
        return true;
    if(this->con!=0) {
        this->con->disconnect();
        //delete this->con;
        //this->con=0;
    }

    printf("MainWindow::connect: this->connString=connString...\n");
    this->connString=connString;
    printf("MainWindow::connect: creo connessione...\n");
    if(this->con==0)
        this->con = DBLayer::createConnection( connString.toStdString().c_str() );
    printf("MainWindow::connect: creo connessione... OK\n");
    return this->con->connect();
}

void MainWindow::runMyThread(QString connString, QString connUser, QString connPwd) {
    printf("%0lx::MainWindow::runMyThread: start %s.\n", (unsigned long) QThread::currentThread(), ((MyThread*)QThread::currentThread())->nomeThread.toStdString().c_str());
    //Connection* con = DBLayer::createConnection( connString.toStdString().c_str() );
    //con->connect();
    Connection* con = this->con;
    this->testLogin(connString, connUser, connPwd, con);
    this->testCRUD(connString, con);
    //con->disconnect();
    printf("%0lx::MainWindow::runMyThread: end %s.\n", (unsigned long) QThread::currentThread(), ((MyThread*)QThread::currentThread())->nomeThread.toStdString().c_str());
}

// ************************* SLOTS

void MainWindow::slotTestThreads() {
    this->clearText();
    this->writeLine("Test Threads");
    this->writeLine("============");
    this->writeLine();
    int num_threads = this->ui->spinBox->value();
    for(int i=0; i<num_threads; i++) {
        QString nomeThread = QString("T%1").arg(i);
        if(!this->mythreads.contains(nomeThread)) {
            this->writeLine( "Creating "+nomeThread+"..." );
            this->mythreads[nomeThread] = new MyThread();
            this->mythreads[nomeThread]->c=this;
            this->mythreads[nomeThread]->nomeThread=nomeThread;
            this->mythreads[nomeThread]->connString = this->ui->comboBox->currentText();
            this->mythreads[nomeThread]->connUser = this->ui->lineUser->text();
            this->mythreads[nomeThread]->connPassword = this->ui->linePwd->text();
            this->mythreads[nomeThread]->myjob=&MainWindow::runMyThread;
            QObject::connect(this->mythreads[nomeThread],SIGNAL(started()),this,SLOT(slotThreadStarted()));//, Qt::DirectConnection);
            QObject::connect(this->mythreads[nomeThread],SIGNAL(finished()),this,SLOT(slotThreadCompleted()));//, Qt::DirectConnection);
        }
        if(this->mythreads[nomeThread]->isRunning()) {
            this->writeLine( "Thread "+nomeThread+" is still running." );
            continue;
        }
        this->writeLine( "Starting Thread "+nomeThread+"..." );
        printf("%0lx::MainWindow::slotTestThreads: starting thread with syncCall...\n", (unsigned long) QThread::currentThread());
        this->mythreads[nomeThread]->start();
        this->writeLine( "Thread "+nomeThread+" started." );
    }
}
void MainWindow::slotTest() {
    this->clearText();

    this->writeLine("Test DBLayer");
    this->writeLine("============");
    this->writeLine("");
/*
    this->writeLine("---------------->>  testDateField");
    this->testDateField();
    this->printFieldCreatiDistrutti();
    this->writeLine("");
*/
    this->writeLine("---------------->>  testDBConnection");
    this->testDBConnection( this->ui->comboBox->currentText(), this->ui->comboQuery->currentText() );
    this->printFieldCreatiDistrutti();
    this->writeLine("");

    this->writeLine("---------------->>  testLogin");
    this->testLogin( this->ui->comboBox->currentText(), this->ui->lineUser->text(), this->ui->linePwd->text() );
    this->printFieldCreatiDistrutti();
    this->writeLine("");
/*
    this->writeLine("---------------->>  testGetKeys");
    this->testGetKeys( this->ui->comboBox->currentText(), this->ui->comboQuery->currentText().split(" ")[3] );
    this->printFieldCreatiDistrutti();
    this->writeLine("");

    this->writeLine("---------------->>  testGetForeignKeys");
    this->testGetForeignKeys( this->ui->comboBox->currentText(), this->ui->comboQuery->currentText().split(" ")[3] );
    this->printFieldCreatiDistrutti();
    this->writeLine("");
*/
    this->writeLine("---------------->>  testDBMgr");
    this->testDBMgr( this->ui->comboBox->currentText(), this->ui->comboQuery->currentText(), this->ui->comboQuery->currentText().split(" ")[3] );
    this->printFieldCreatiDistrutti();
    this->writeLine("");

    this->writeLine("---------------->>  testDBE");
    this->testDBE( this->ui->comboBox->currentText() );
    this->printFieldCreatiDistrutti();
    this->writeLine("");

    this->writeLine("---------------->>  testCRUD");
    this->testCRUD( this->ui->comboBox->currentText() );
    this->printFieldCreatiDistrutti();
    this->writeLine("");
}
void MainWindow::slotTestSchema() {
    this->clearText();

    this->writeLine("Test Schema");
    this->writeLine("===========");
    this->writeLine("");

    this->writeLine("---------------->>  testLogin");
    this->testLogin( this->ui->comboBox->currentText(), this->ui->lineUser->text(), this->ui->linePwd->text() );
    this->printFieldCreatiDistrutti();
    this->writeLine("");

    this->writeLine("---------------->>  testSchemaName");
    this->testSchemaName(this->ui->comboBox->currentText() );
    this->printFieldCreatiDistrutti();
    this->writeLine("");

    this->writeLine("---------------->>  testGetFormSchema");
    this->testGetFormSchema( this->ui->comboBox->currentText(), "cpp" );
    this->printFieldCreatiDistrutti();
    this->writeLine("");

    this->writeLine("---------------->>  testGetDBSchema");
    this->testGetDBSchema( this->ui->comboBox->currentText(), "cpp" );
    this->printFieldCreatiDistrutti();
    this->writeLine("");
}

void MainWindow::slotConnected(bool b) {
    this->writeLine(b ? "Connected" : "Disconnected");
    this->statusBar()->setStatusTip( b ? "Connected" : "Disconnected" );
}
void MainWindow::slotResultSetReady(int req) {
    QXmlrpcResultSet* rs;
    if(this->res.find(req)==this->res.end()) {
        printf("MainWindow::slotResultSetReady: ERROR - req=%d not found!\n",req);
        return;
    }
    rs = (QXmlrpcResultSet*) this->res[req];
    this->res.erase( this->res.find(req) );

    printf("MainWindow::slotResultSetReady: req=%d\n",req);
    printf("MainWindow::slotResultSetReady: res=%d\n",rs->getReqNumber());
    printf("MainWindow::slotResultSetReady: res=%s\n",rs->toString("\n").c_str());

    this->appendText( QString("---------------> req %1 \n").arg(req) );

    int nColonne = rs->getNumColumns();
    this->writeLine( QString("N. Colonne: %1").arg(nColonne ) );
    for( int i=0; i<nColonne; i++) {
        this->appendText( QString("Colonna[%1]: ").arg(i) );
        this->appendText( QString(rs->getColumnName(i).c_str()) );
        this->appendText( QString(" - %1").arg(rs->getColumnType(i).c_str()) );
        this->appendText( QString(": %1\n").arg(rs->getColumnSize(i)) );
    }

    int nRighe = rs->getNumRows();
    this->writeLine( QString(" Righe: %1").arg(nRighe) );
    for(int r=0; r<nRighe; r++) {
        for(int c=0; c<nColonne; c++) {
            if (! rs->isNull(r,c) ) {
                this->appendText( QString("%1\t").arg(rs->getValue(r,c).c_str()) );
            } else {
                this->appendText( QString("\\N\t") );
            }
        }
        this->writeLine();
    }
}
void MainWindow::slotResultSetError(int req,QString errorMsg) {
    this->writeLine( QString("%1%").arg(req) );
    this->writeLine(errorMsg);
    this->statusBar()->setToolTip( errorMsg );
}

void MainWindow::slotThreadStarted() {
    //printf("%0x::MainWindow::slotThreadStarted: start.\n", (unsigned int) QThread::currentThread());
    this->threadsRunning++;
    this->writeLine( QString("Threads running: %1").arg(this->threadsRunning));
    //printf("%0x::MainWindow::slotThreadStarted: Threads running=%d\n", (unsigned int) QThread::currentThread(),this->threadsRunning);
    //printf("%0x::MainWindow::slotThreadStarted: end.\n", (unsigned int) QThread::currentThread());
}
void MainWindow::slotThreadCompleted() {
    //printf("%0x::MainWindow::slotThreadCompleted: start.\n", (unsigned int) QThread::currentThread());
    this->threadsRunning--;
    //printf("%0x::MainWindow::slotThreadCompleted: Threads running=%d\n", (unsigned int) QThread::currentThread(),this->threadsRunning);
    this->writeLine( QString("Threads running: %1").arg(this->threadsRunning));
    //printf("%0x::MainWindow::slotThreadCompleted: end.\n", (unsigned int) QThread::currentThread());
}

void MainWindow::slotAppendText(QString s) {
    this->appendText(s);
}
void MainWindow::slotClearText() {
    this->clearText();
}
