#ifndef QXMLRPCRESULTSET_H
#define QXMLRPCRESULTSET_H

#include <dblayer/dblayer.h>
#include <dblayer/dbconnection.h>
using namespace DBLayer;


namespace DBLayer {

class QXmlrpcResultSet : public ResultSet {
  friend class QXmlrpcConnection;

  public:
    QXmlrpcResultSet();
    virtual ~QXmlrpcResultSet();
//	virtual int getColumnIndex(string* columnName );
    virtual int getColumnSize(int i) const;

    virtual string toString(string prefix="\n") const;

    int getReqNumber();

  protected:
    int req;

  private:
    static std::string integer2string(long longValue);
    static std::string double2string(double longValue);
};

}

#endif // QXMLRPCRESULTSET_H
