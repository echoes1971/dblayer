/***************************************************************************
**	dbentity.cpp  v0.1.0 - 2012.03.19
**	-----------------------------------
**
**	Author:		Roberto Rocco Angeloni.
**	E-mail:		roberto@roccoangeloni.it
**	Comment:
**	To Do:
**	Future:
**	History:
**		v0.0.1 - 2002.10.20	First Old Version
**		v0.1.0 - 2006.05.04 Rewritten for the new framework
**
** @copyright &copy; 2011-2014 by Roberto Rocco Angeloni <roberto@roccoangeloni.it>
** @license http://opensource.org/licenses/lgpl-3.0.html GNU Lesser General Public License, version 3.0 (LGPLv3)
** @version $Id: dbentity.cpp $
** @package rproject::dblayer
**
** Permission to use, copy, modify, and distribute this software for any
** purpose with or without fee is hereby granted, provided that the above
** copyright notice and this permission notice appear in all copies.
**
** THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
** WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
** MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
** ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
** WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
** ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
** OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
****************************************************************************/

#include <stdlib.h>
#include <cstdio> // For sprintf in DBEntity::uuid2hex

#include "dbentity.h"
using namespace DBLayer;

ColumnDefinitions DBEntity::_columns;
ForeignKeyVector DBEntity::_fkv;

DBEntity::DBEntity() {
    this->tableName = this->tableName.append( "no table name" );
}
DBEntity::DBEntity(const string* tableName) {
    this->tableName.clear();
    this->tableName.append( tableName->c_str() );
}
DBEntity::~DBEntity() {}
const string* DBEntity::getSchemaName() { return 0; }
string DBEntity::getTableName() const { return string(tableName); }

string DBEntity::name() const { return "DBEntity"; }

string DBEntity::toString_nodes(string prefix) const {
    string ret( prefix + "<" );
    ret.append( this->name() ); ret.append( " " );
    // Keys
    DBFieldVector* myKeys = this->getKeys();
    if( myKeys->size()>0 ) {
        ret.append("_keys=\'{");
        unsigned int numeroChiavi = (unsigned int) myKeys->size();
        for(unsigned int i=0; i<numeroChiavi; i++) {
            ret.append( myKeys->at(i)->getName() );
            if( i!= (numeroChiavi-1) ) {
                ret.append(",");
            }
        }
        ret.append("}\' ");
    }
    ret.append(">");
    // Fields
    for(unsigned int i=0; i<fields.size(); i++) {
        ret.append( prefix + " <" + fields[i]->getName() + " " );
        if( this->isKey( fields[i]->getName() ) ) {
            ret.append( "isKey=\'true\' " );
        }
        ret.append("type=\'");
        switch(fields[i]->getType()) {
          case Field::STRING:
            ret.append( "string" );
            break;
          case Field::INTEGER:
            ret.append( "integer" );
            break;
          case Field::FLOAT:
            ret.append( "float" );
            break;
          case Field::BOOLEAN:
            ret.append( "boolean" );
            break;
          case Field::DATE:
            ret.append( "date" );
            break;
          default:
            ret.append("unknown");
            break;
        }
        ret.append("\' ");
        ret.append(">");
        switch(fields[i]->getType()) {
          case Field::STRING:
            ret.append( fields[i]->getStringValue()->c_str() );
            break;
          case Field::INTEGER:
            ret.append( DBLayer::integer2string( fields[i]->getIntegerValue() ).c_str() );
            break;
          case Field::FLOAT:
            ret.append( fields[i]->toString().c_str() );
            break;
          case Field::BOOLEAN:
            ret.append( fields[i]->toString().c_str() );
            break;
          case Field::DATE:
            ret.append( fields[i]->toString().c_str() );
            break;
        }
        ret.append( "</" + fields[i]->getName() + ">" );
    }

    ret.append(prefix+"<!--");

    ret.append(prefix+"{");
    // Table Columns
    if(this->getColumns().size()>0) {
        ret.append(prefix+" 'columns': {");
        for(const pair<string,vector<string> > pair : this->getColumns()) {
            ret.append(prefix+"  '").append(pair.first).append("': [");
            for(const string s : pair.second) {
                ret.append("'").append(s).append("',");
            }
            ret.append("],");
        }
        ret.append(prefix+" },");
    }
    // Foreign Keys
    if(this->getFK().size()>0) {
        ret.append(prefix+" 'foreignkeys': [");
        for(const ForeignKey fk : this->getFK()) {
            ret.append(prefix+"  {");
            ret.append(prefix+"   'fk_column':'"+fk.colonna_fk+"',");
            ret.append(prefix+"   'referred_table':'"+fk.tabella_riferita+"',");
            ret.append(prefix+"   'referred_column':'"+fk.colonna_riferita+"',");
            ret.append(prefix+"  },");
            //colonna_fk, string tabella_riferita, string colonna_riferita
        }
        ret.append(prefix+" ],");
    }
    // Order by
    if(this->getOrderBy().size()>0) {
        ret.append(prefix+" 'order_by': [");
        for(const string& s : this->getOrderBy())
            ret.append("'").append(s).append("',");
        ret.append("],");
    }

    ret.append(prefix+"}");
    ret.append(prefix+"-->");

    ret.append( prefix + "</" + this->name() + ">" );
    return ret;
}

string DBEntity::toString(string prefix, bool valuesAsAttributes) const {
    if ( !valuesAsAttributes) {
        return toString_nodes(prefix);
    }
    string ret( prefix + "<" );
    ret.append( this->name() ); ret.append( " " );
    // Keys
    DBFieldVector* myKeys = this->getKeys();
    if( myKeys->size()>0 ) {
        ret.append("_keys=\'{");
        unsigned int numeroChiavi = (unsigned int) myKeys->size();
        for(unsigned int i=0; i<numeroChiavi; i++) {
            ret.append( myKeys->at(i)->getName() );
            if( i!= (numeroChiavi-1) ) {
                ret.append(",");
            }
        }
        ret.append("}\' ");
    }
    // Fields
    for(unsigned int i=0; i<fields.size(); i++) {
        ret.append( fields[i]->getName().c_str() );
        ret.append( "=\'" );
        switch(fields[i]->getType()) {
          case DBField::STRING:
            ret.append( fields[i]->getStringValue()->c_str() );
            break;
          case DBField::INTEGER:
            ret.append( DBLayer::integer2string( fields[i]->getIntegerValue() ).c_str() );
            break;
          case DBField::FLOAT:
            ret.append( fields[i]->toString().c_str() );
            break;
          case DBField::BOOLEAN:
            ret.append( fields[i]->toString().c_str() );
            break;
          case DBField::DATE:
            ret.append( fields[i]->toString().c_str() );
            break;
        }
        ret.append( "\' " );
    }
    return ret + "/>";
}

void* DBEntity::getValue(string* field) {
  for(unsigned int i=0; i<fields.size() ; i++) {
    if (fields[i]->getName().compare(field->c_str()) == 0) {
      switch(fields[i]->getType()) {
        case Field::STRING:
        break;
        case Field::INTEGER:
          return (void*) fields[i]->getIntegerValue();
        break;
        case Field::FLOAT:
        break;
        case Field::DATE:
// TODO         return fields[i]->getDateValue();
        break;
      }
      i = (unsigned int) fields.size();
    }
  }
  return (void*)0;
}
void* DBEntity::getValue(const string& field) {
  for(unsigned int i=0; i<fields.size(); i++) {
    if (fields[i]->getName().compare(field.c_str()) == 0) {
      switch(fields[i]->getType()) {
        case DBField::STRING:
        break;
        case DBField::INTEGER:
          return (void*) fields[i]->getIntegerValue();
        break;
        case DBField::FLOAT:
        break;
        case DBField::DATE:
// TODO         return fields[i]->getDateValue();
        break;
      }
      i = (unsigned int)fields.size();
    }
  }
  return (void*)0;
}

string DBEntity::getStringValue(const string* fieldName) {
    Field* ret = this->getField(fieldName);
    return ret==0 ? "" : ret->toString();
}

DBFieldVector* DBEntity::getKeys() const {
    static DBFieldVector vuoto;
    return &vuoto;
}
DBLayer::StringVector DBEntity::getKeyNames() const {
    StringVector ret;
    DBFieldVector* chiavi = this->getKeys();
    unsigned int chiaviSize = (unsigned int)chiavi->size();
    for(unsigned int i=0; i<chiaviSize; i++) {
        DBField* chiave = chiavi->at(i);
        ret.push_back( chiave->getName() );
    }
    return ret;
}
bool DBEntity::isKey(string fieldName) const {
    StringVector chiavi = this->getKeyNames();
    bool found = false;
    for(unsigned int i=0; i<chiavi.size() && !found; i++) {
        found = ( fieldName==chiavi[i] );
    }
    return found;
}

DBEntity* DBEntity::createNewInstance() const { return new DBEntity(); }
Field* DBEntity::createNewField(const string* fieldName, bool valore) const { return new BooleanField(fieldName, valore); }
Field* DBEntity::createNewField(const string* fieldName, float valore) const { return new FloatField(fieldName, valore); }
Field* DBEntity::createNewField(const string* fieldName, long valore) const { return new IntegerField(fieldName, valore); }
Field* DBEntity::createNewField(const string* fieldName, const string* valore) const { return new StringField(fieldName, valore); }
Field* DBEntity::createNewDateField(const string* fieldName, const string* valore) const { return new DateField(fieldName, valore); }

ForeignKeyVector& DBEntity::getFK() const { return DBEntity::_fkv; }
ForeignKeyVector DBEntity::getFKForTable(string tablename) {
    ForeignKeyVector& fks = this->getFK();
    ForeignKeyVector ret;
    for(unsigned int i=0; i<fks.size(); i++) {
        ForeignKey& f = fks.at(i);
        string tabella_riferita = f.tabella_riferita;
        if( tabella_riferita == tablename ) {
            ret.push_back( f );
        }
    }
    return ret;
}
void DBEntity::readFKFrom(DBEntity* dbe) {
    ForeignKeyVector fks = this->getFKForTable( dbe->getTableName() );
    for(unsigned int i=0; i<fks.size(); i++) {
        ForeignKey& f = fks.at(i);
        DBField* myfield = (DBField*) dbe->getField( &f.colonna_riferita );
        if( myfield!=0 && !myfield->isNull() ) {
            string nomeField = f.colonna_fk;
            if ( myfield->isBoolean() ) {
                    this->setValue( &nomeField, myfield->getBooleanValue() );
            } else if ( myfield->isInteger() ) {
                    this->setValue( &nomeField, myfield->getIntegerValue() );
            } else if ( myfield->isFloat() || myfield->isDouble() )  {
                    this->setValue( &nomeField, myfield->getFloatValue() );
            } else if ( myfield->isString() ) {
                    this->setValue( &nomeField, myfield->getStringValue() );
            } else if ( myfield->isDate() ) {
                    this->setDateValue( &nomeField, myfield->getStringValue() );
            }
        }
    }
}
DBEntity* DBEntity::writeFKTo(DBEntity* dbemaster) {
    ForeignKeyVector fks = this->getFKForTable( dbemaster->getTableName() );
    for(unsigned int i=0; i<fks.size(); i++) {
        ForeignKey& f = fks.at(i);
        DBField* myfield = (DBField*) this->getField( &f.colonna_fk );
        if( myfield!=0 && !myfield->isNull() ) {
            string nomeField = myfield->getName();
            if ( myfield->isBoolean() ) {
                    dbemaster->setValue( &nomeField, myfield->getBooleanValue() );
            } else if ( myfield->isInteger() ) {
                    dbemaster->setValue( &nomeField, myfield->getIntegerValue() );
            } else if ( myfield->isFloat() || myfield->isDouble() )  {
                    dbemaster->setValue( &nomeField, myfield->getFloatValue() );
            } else if ( myfield->isString() ) {
                    dbemaster->setValue( &nomeField, myfield->getStringValue() );
            } else if ( myfield->isDate() ) {
                    dbemaster->setDateValue( &nomeField, myfield->getStringValue() );
            }
        }
    }
    return dbemaster;
}
bool DBEntity::isFK(string field_name, string referred_table) {
    bool found = false;
    ForeignKeyVector& fks = this->getFK();
    if( referred_table.length()>0 ) {
        fks = this->getFKForTable( referred_table );
    }
    for(unsigned int i=0; i<fks.size() && !found; i++) {
        ForeignKey& f = fks.at(i);
        found = ( f.colonna_fk == field_name );
    }
    return found;
}

void DBEntity::cleanKeyFields() {
    bool cancellato = true;
    while(cancellato) {
//        for(const auto& elem : fields) {
//            string nomeField = elem->getName();
//            if( this->isKey( nomeField ) ) {
//                delete elem;
//                fields.erase( (*elem) );
//                cancellato = true;
//                break;
//            } else {
//                cancellato = false;
//            }
//        }
        for(FieldVector::iterator it=fields.begin(); it!=fields.end(); it++) {
            string nomeField = (*it)->getName();
            if( this->isKey( nomeField ) ) {
                delete (*it);
                fields.erase(it);
                cancellato = true;
                break;
            } else {
                cancellato = false;
            }
        }
    }
}

string DBEntity::getColumnType(const string& column_name) {
    ColumnDefinitions::iterator it = this->_columns.find(column_name);
    if(it!=this->_columns.end()) {
        string search = (*it).second[0];
        int parentesi = search.find('(');
        if(parentesi>0) {
            search = search.substr(0,parentesi);
        }
        if(search=="char" || search=="varchar" || search=="text")
            return "string";
        else
            return search;
    } else {
        return "";
    }
}
ColumnDefinitions DBEntity::getColumns() const { return DBEntity::_columns; }
string DBEntity::dbeType2dbType(const string& dbetype) {
    string ret = dbetype;
    if(dbetype=="int")
        ret = "int(11)";
    else if(dbetype=="uuid")
        ret = "varchar(16)";
    return ret;
}
string DBEntity::dbType2dbeType(const string& dbtype) {
    string ret = dbtype;
    if(dbtype=="int(11)")
        ret = "int";
    else if(dbtype=="varchar(16)")
        ret = "uuid";
    return ret;
}

string DBEntity::dbConstraints2dbeConstraints(map<string,string>& def) {
    StringVector constraints;
    if( def["Null"]=="NO") constraints.push_back("not null");
    if( def.find("Default")!=def.end() ) {
        string apice ="\'";
        if( def["Type"]=="int(11)" || def["Type"]=="float")
            apice = "";
        constraints.push_back( string("default ").append(apice).append(def["Default"]).append(apice) );
    } else if(def.find("Default")==def.end() && def["Null"]=="YES") {
        constraints.push_back("default null");
    }
    string glue(" ");
    return constraints.size()>0 ? DBLayer::joinString(&constraints, &glue) : "";
}
string DBEntity::dbColumnDefinition2dbeColumnDefinition(map<string,string>& def) {
    string constraints = DBEntity::dbConstraints2dbeConstraints(def);
    return string("'$col'=>array('").append( DBEntity::dbType2dbeType(def["Type"]) ).append("',")
            .append( constraints.length()>0 ? constraints : "" )
            .append("),\n");
}
DBLayer::StringVector DBEntity::getOrderBy() const { return this->getKeyNames(); }
string DBEntity::getOrderByString() const {
    string glue(",");
    DBLayer::StringVector tmp = this->getOrderBy();
    return DBLayer::joinString(&tmp, &glue);
}
string DBEntity::uuid2hex(const string& str) {
    if(str.length()==0) return str;
    int str_len = str.length();
    if(str_len<4) return str;
    if(str.substr(0,4)=="uuid")
        return str;
    string hex = "";
    int i = 0;
    do {
        char tmp[20];
    #if defined( WIN32 ) && ! defined( USING_GCC_ON_WIN32 )
        sprintf_s(tmp,"%x",str.c_str()[i]);
    #else
        sprintf(tmp,"%x",str.c_str()[i]);
    #endif
        hex.append(tmp);
        i++;
    } while (i<str_len);
    return string("uuid").append(hex);
}
string DBEntity::hex2uuid(const string& str) {
    int str_len = str.length();
    if(str_len==0 || str_len<4 || str.substr(0,4)!="uuid")
        return str;
    string tmp = str.substr(4);
    string bin;
    int i = 0;
    str_len = tmp.length();
    do {
        unsigned long x = strtoul( tmp.substr(i,2).c_str(), 0, 16 );
        char c = char(x);
        bin.append( &c );
        i+=2;
    } while(i<str_len);
    return bin;
}

bool DBEntity::isNew() {
    bool ret = false;
    DBLayer::StringVector nomiChiavi = this->getKeyNames();
    for(const string& elem : nomiChiavi) {
        Field* field = this->getField( &elem );
        if(field==0) ret=true;
        else if(field->isNull()) ret=true;
    }
    return ret;
}

void DBEntity::_before_insert(DBMgr* dbmgr) { if(dbmgr==0) return; }
void DBEntity::_after_insert(DBMgr* dbmgr) { if(dbmgr==0) return; }

void DBEntity::_before_update(DBMgr* dbmgr) { if(dbmgr==0) return; }
void DBEntity::_after_update(DBMgr* dbmgr) { if(dbmgr==0) return; }

void DBEntity::_before_delete(DBMgr* dbmgr) { if(dbmgr==0) return; }
void DBEntity::_after_delete(DBMgr* dbmgr) { if(dbmgr==0) return; }

void DBEntity::_before_copy(DBMgr* dbmgr) { if(dbmgr==0) return; }
void DBEntity::_after_copy(DBMgr* dbmgr) { if(dbmgr==0) return; }
