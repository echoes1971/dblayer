#ifndef QXMLRPCRESULTSET_H
#define QXMLRPCRESULTSET_H

#include <dblayer/dblayer.h>
#include <dblayer/dbconnection.h>
using namespace DBLayer;


namespace DBLayer {

class QXmlrpcResultSet : public ResultSet {
  friend class QXmlrpcConnection;

  private:
    static std::string integer2string(long longValue);
    static std::string double2string(double longValue);
  protected:
    int req;
  public:
    QXmlrpcResultSet();
    virtual ~QXmlrpcResultSet();
//	virtual int getColumnIndex(string* columnName );
    virtual int getColumnSize(int i);

    virtual string toString(string prefix="\n");

    int getReqNumber();
};

}

#endif // QXMLRPCRESULTSET_H
