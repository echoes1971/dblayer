/***************************************************************************
**	dbschema.h  v0.0.1 - 2006.05.04
**	-------------------------------
**
**	Author:		Roberto Rocco Angeloni.
**	email:		roberto@roccoangeloni.it
**	Copyright:	(c) 2006 by Roberto Rocco Angeloni.
**	Comment:
**	To Do:
**	Future:
**	History:
**		v0.0.1 - 2006.04.XX	...
****************************************************************************/

#ifndef MYSCHEMA_DBETESTDBLAYER_H
#define MYSCHEMA_DBETESTDBLAYER_H

#include "dblayer/dblayer.h"
#include "dblayer/dbentity.h"
#include "dblayer/dbfield.h"
#include "dblayer/dbmgr.h"
using namespace DBLayer;

#include <string>
using namespace std;

namespace TestSchema {

	class DECLSPECIFIER DBETestDBLayer : public DBEntity {
	  public:
		DBETestDBLayer();
		virtual ~DBETestDBLayer();

        virtual ColumnDefinitions getColumns() const;

        virtual string name() const;
        virtual StringVector getKeys() const;
        virtual DBETestDBLayer* createNewInstance() const;

        virtual void _before_insert(DBMgr* dbmgr);
	  private:
        static ColumnDefinitions _columns;
        static StringVector chiavi;
    };

	class DECLSPECIFIER DBESocieta : public DBEntity {
	  public:
		DBESocieta();
		virtual ~DBESocieta();

        virtual ColumnDefinitions getColumns() const;

        virtual string name() const;
        virtual StringVector getKeys() const;
        virtual DBESocieta* createNewInstance() const;

        virtual void _before_insert(DBMgr* dbmgr=0);
        virtual void _before_copy(DBMgr* dbmgr=0);
	  private:
        static ColumnDefinitions _columns;
        static StringVector chiavi;
    };

    string getSchema();
    void registerClasses(DBEFactory* dbeFactory);
    void checkDB(DBMgr& dbmgr, bool verbose=false);
}

#endif
