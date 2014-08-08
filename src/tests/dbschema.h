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

namespace MySchema {

	class DECLSPECIFIER DBETestDBLayer : public DBEntity {
	  public:
		DBETestDBLayer();
		virtual ~DBETestDBLayer();

		virtual string name();
		virtual DBFieldVector* getKeys();
		virtual DBETestDBLayer* createNewInstance();

	  private:
		static const string nomiCampiChiave[];
		static IntegerField chiave1;
		static DBFieldVector chiavi;
	};

	class DECLSPECIFIER DBESocieta : public DBEntity {
	  public:
		DBESocieta();
		virtual ~DBESocieta();

		virtual string name();
		virtual DBFieldVector* getKeys();
		virtual DBESocieta* createNewInstance();

        virtual void _before_insert(DBMgr* dbmgr=0);
        virtual void _before_copy(DBMgr* dbmgr=0);
	  private:
		static const string nomiCampiChiave[];
		static IntegerField chiave1;
		static DBFieldVector chiavi;
	};

}

#endif
