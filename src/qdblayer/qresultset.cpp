#include "qresultset.h"
using namespace DBLayer;

#ifdef WIN32
QResultSet::QResultSet() : ResultSet() {
#else
QResultSet::QResultSet() : ResultSet::ResultSet() {
#endif
    //cout << "QResultSet::QResultSet: inizio." << endl;
    //cout << "QResultSet::QResultSet: fine." << endl;
}
QResultSet::~QResultSet() {
    //cout << "QResultSet::~QResultSet: inizio." << endl;
    //cout << "QResultSet::~QResultSet: fine." << endl;
}

int QResultSet::getColumnSize(int i) const {
    // Non significativo per Xmlrpc
    return -i;
}

string QResultSet::toString(string prefix) const {
        string ret;
        ret.append(prefix+"<QResultSet>");

        int nColonne = this->getNumColumns();
        ret.append(prefix+" <Columns>" );
        for( int i=0; i<nColonne; i++) {
                ret.append(prefix+"  <Column ");
                ret.append("position=\'"+DBLayer::integer2string((long)i)+"\' ");
                ret.append("name=\'" + this->getColumnName(i)+"\' ");
                ret.append("type=\'" + this->getColumnType(i)+"\' ");
                ret.append("size=\'" + DBLayer::integer2string((long)this->getColumnSize(i))+"\' ");
                ret.append("/>");
        }
        ret.append(prefix+" </Columns>" );

        ret.append(prefix+" <Rows>");
        int nRighe = this->getNumRows();
        for(int r=0; r<nRighe; r++) {
                ret.append(prefix+"  <Row num=\'"+DBLayer::integer2string((long)r)+"\'>");
                for(int c=0; c<nColonne; c++) {
                        string nomeColonna = this->getColumnName(c);
                        if (! this->isNull(r,c) ) {
                                ret.append(prefix+"   <"+nomeColonna+">");
                                ret.append( this->getValue(r,c) );
                                ret.append("</"+nomeColonna+">");
                        } else {
                                ret.append(prefix+"   <"+nomeColonna+" />");
                        }
                }
                ret.append(prefix+"  </Row>");
        }
        ret.append(prefix+" </Rows>");

        ret.append(prefix+"</QResultSet>");
        return ret;
}

std::string QResultSet::integer2string(long longValue) {
        int i = 50-2;
        char tmp[50];  tmp[50-2] = '0';  tmp[50-1] = '\0';
        if ( longValue<0 ) {
                std::string tmpString = std::string( "-" );
                tmpString.append( integer2string( -1 * longValue ) );
                return tmpString;
        }
        if (longValue>=0 && longValue<10) {
                tmp[i] = (char) ((int)longValue%10)+'0';
                i--;
        } else {
                for (i=50-2; i>=0 && longValue>0 ; i--) {
                        tmp[i] = (char) ((int)longValue%10)+'0';
                        longValue = longValue / 10;
                }
        }
        return std::string( (char*)&tmp[i+1] );
}
std::string QResultSet::double2string(double longValue) {
        int i = 50-2;
        char tmp[50];  tmp[50-2] = '0';  tmp[50-1] = '\0';
        if ( longValue<0 ) {
                std::string tmpString = std::string( "-" );
                tmpString.append( double2string( -1 * longValue ) );
                return tmpString;
        }
        std::string parte_intera;
        if ( longValue>=1 ) {
                parte_intera.append( integer2string( (long) longValue ) );
        }
        double decimale = longValue - ((long)longValue);
        if( decimale>0 && decimale<1 ) {
                parte_intera.append( "." );
                long decimale_long = 0;
                int num_decimali = 5;
                while( decimale>0 && decimale<1 && decimale!=0.0 && num_decimali>0 ) {
                        decimale_long = decimale_long*10 + (long)(decimale*10);
                        decimale = (decimale*10) - (long)(decimale*10);
                        num_decimali--;
                }
                parte_intera.append( integer2string( decimale_long ) );
        }
        return parte_intera + std::string( (char*)&tmp[i+1] );
}
