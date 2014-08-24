#ifndef QXMLRPCRESULTSET_H
#define QXMLRPCRESULTSET_H

#include <dblayer/dblayer.h>
#include <dblayer/dbconnection.h>
using namespace DBLayer;


namespace DBLayer {

class QResultSet : public ResultSet {
  friend class QSqliteConnection;
  friend class QXmlrpcConnection;

  public:
    QResultSet();
    virtual ~QResultSet();
    virtual int getColumnSize(int i) const;

    virtual string toString(string prefix="\n") const;

  private:
    static std::string integer2string(long longValue);
    static std::string double2string(double longValue);
};

}

#endif // QXMLRPCRESULTSET_H
