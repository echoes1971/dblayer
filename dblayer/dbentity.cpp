/***************************************************************************
**	dbentity.cpp  v0.1.0 - 2006.05.04
**	---------------------------------
**
**	Author:		Roberto Rocco Angeloni.
**	email:		roberto@roccoangeloni.it
**	Comment:
**	To Do:
**	Future:
**	History:
**		v0.0.1 - 2002.10.20	First Old Version
**		v0.1.0 - 2006.05.04 Rewritten for the new framework
**
** @copyright &copy; 2011 by Roberto Rocco Angeloni <roberto@roccoangeloni.it>
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

#include "dbentity.h"
using namespace DBLayer;


DBEntity::DBEntity() {
    this->tableName = this->tableName.append( "no table name" );
}
DBEntity::DBEntity(const string* tableName) {
    this->tableName.clear();
    this->tableName.append( tableName->c_str() );
}
DBEntity::~DBEntity() {
	// Realizzato dalla superclasse
/*	cout << "DBEntity::~DBEntity: tableName \'" << this->tableName
			<< "\' deleting " << this->fields.size() << "  fields..." << endl;
	// Elimino i fields
	for(unsigned int i=0; i<this->fields.size(); i++) {
		cout << "DBEntity::~DBEntity:\t" << this->fields.at(i)->getName() << endl;
		delete this->fields.at(i);
	}
	cout << "DBEntity::~DBEntity: =====" << endl;*/
}
const string* DBEntity::getSchemaName() { return 0; }
string DBEntity::getTableName() {
    return string(tableName);
}

/* TO DO
bool DBEntity::mapFromRS(ResultSet *rs) {
  try {
#ifndef __WIN32
    for(uint i=0; i<fields.size(); i++) {
#else
    for(int i=0; i<fields.size(); i++) {
#endif
//      this->setValue( fields[i]->Name , new string( rs->getString( fields[i]->Name->c_str() ) ) );
      string cippa = string("");
      string* lippa = fields[i]->Name;
      int indice = rs->findColumn( string( fields[i]->Name->c_str() ) );
      switch ( fields[i]->getType() ) {
          case DBField::STRING:
              cout << "DBEntity::mapFromRS: name=" << fields[i]->Name->c_str() <<endl;
              cippa += rs->getString( indice );
              printf( "--\'%s\'\n", rs->getString( 2 ).c_str()  );
              cout << "DBEntity::mapFromRS: value="
                   << rs->getString( indice )
//                   << rs->getString( string( fields[i]->Name->c_str() ) )
                   << endl;
//            fields[i]->setStringValue( rs->getString( indice ) );
            fields[i]->setStringValue( new string( rs->getString( indice ).c_str() ) );
//            fields[i]->setStringValue( new string( rs->getString( fields[i]->Name->c_str() ) ) );
          break;
          case DBField::INTEGER:
            fields[i]->setIntegerValue( rs->getLong( fields[i]->Name->c_str() ) );
          break;
          case DBField::FLOAT:
            fields[i]->setFloatValue( rs->getFloat( fields[i]->Name->c_str() ) );
          break;
          case DBField::DATE:
            // ATTENZIONE: taking address of temporary
//            fields[i]->setDateValue( new odbc::Date( rs->getDate( fields[i]->Name->c_str() ) ) );
          break;
      }
    }
    return true;
  } catch(SQLException& e) {
    cerr << endl << e.getMessage() << endl;
    return false;
  }
}
*/

string DBEntity::name() { return "DBEntity"; }

string DBEntity::toString_nodes(string prefix) {
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
	ret.append( prefix + "</" + this->name() + ">" );
	return ret;
}

string DBEntity::toString(string prefix, bool valuesAsAttributes) {
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
//          return fields[i]->getStringValue();
        break;
        case Field::INTEGER:
          return (void*) fields[i]->getIntegerValue();
        break;
        case Field::FLOAT:
//          return fields[i]->getFloatValue();
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
//          return fields[i]->getStringValue();
        break;
        case DBField::INTEGER:
          return (void*) fields[i]->getIntegerValue();
        break;
        case DBField::FLOAT:
//          return fields[i]->getFloatValue();
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

DBFieldVector* DBEntity::getKeys() {
    static DBFieldVector vuoto;
    return &vuoto;
}
DBLayer::StringVector DBEntity::getKeyNames() {
    StringVector ret;
    DBFieldVector* chiavi = this->getKeys();
    unsigned int chiaviSize = (unsigned int)chiavi->size();
    for(unsigned int i=0; i<chiaviSize; i++) {
        DBField* chiave = chiavi->at(i);
        ret.push_back( chiave->getName() );
    }
    return ret;
}
bool DBEntity::isKey(string nomeCampo) {
    StringVector chiavi = this->getKeyNames();
    bool found = false;
    for(unsigned int i=0; i<chiavi.size() && !found; i++) {
        found = ( nomeCampo==chiavi[i] );
    }
    return found;
}

DBEntity* DBEntity::createNewInstance() { return new DBEntity(); }
Field* DBEntity::createNewField(const string* fieldName, bool valore) { return new BooleanField(fieldName, valore); }
Field* DBEntity::createNewField(const string* fieldName, float valore) { return new FloatField(fieldName, valore); }
Field* DBEntity::createNewField(const string* fieldName, long valore) { return new IntegerField(fieldName, valore); }
Field* DBEntity::createNewField(const string* fieldName, const string* valore) { return new StringField(fieldName, valore); }
Field* DBEntity::createNewDateField(const string* fieldName, const string* valore) { return new DateField(fieldName, valore); }

ForeignKeyVector* DBEntity::getFK() {
    static ForeignKeyVector vuoto;
    return &vuoto;
}
ForeignKeyVector DBEntity::getFKForTable(string tablename) {
    ForeignKeyVector* fks = this->getFK();
    ForeignKeyVector ret;
    for(unsigned int i=0; i<fks->size(); i++) {
        ForeignKey* f = fks->at(i);
        string tabella_riferita = f->tabella_riferita;
        if( tabella_riferita == tablename ) {
            ret.push_back( f );
        }
    }
    return ret;
}
void DBEntity::readFKFrom(DBEntity* dbe) {
    ForeignKeyVector fks = this->getFKForTable( dbe->getTableName() );
    for(unsigned int i=0; i<fks.size(); i++) {
        ForeignKey* f = fks.at(i);
        DBField* myfield = (DBField*) dbe->getField( &f->colonna_riferita );
        if( myfield!=0 && !myfield->isNull() ) {
            string nomeField = myfield->getName();
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
        ForeignKey* f = fks.at(i);
        DBField* myfield = (DBField*) this->getField( &f->colonna_fk );
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
bool DBEntity::isFK(string field_name, string tabella_riferita) {
    bool found = false;
    ForeignKeyVector myfks;
    ForeignKeyVector* fks;
    if( tabella_riferita.length()>0 ) {
        myfks = this->getFKForTable( tabella_riferita );
        fks = &myfks;
    } else {
        fks = this->getFK();
    }
    for(unsigned int i=0; i<fks->size() && !found; i++) {
        ForeignKey* f = fks->at(i);
        found = ( f->colonna_fk == field_name );
    }
    return found;
}

void DBEntity::cleanKeyFields() {
    bool cancellato = true;
    while(cancellato) {
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

bool DBEntity::isNew() {
    bool ret = false;
    DBLayer::StringVector nomiChiavi = this->getKeyNames();
    for(DBLayer::StringVector::iterator it=nomiChiavi.begin(); !ret && it!=nomiChiavi.end(); it++) {
        Field* field = this->getField( &(*it) );
        if(field==0) ret=true;//continue;
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
