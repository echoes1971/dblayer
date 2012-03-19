/***************************************************************************
**	dbmgr.h  v0.1.4 - 2006.05.22
**	----------------------------
**
**	Author:		Roberto Rocco Angeloni.
**	email:		roberto@roccoangeloni.it
**	Comment:
**	TODO:		- Testare <b>quoteDate</b> su insert, update e buildKeyCondition
**	Future:
**	History:
**		v0.0.1 - 2002.10.23	First Old Version
**		v0.1.0 - 2006.05.04 Rewritten for the new framework
**		v0.1.1 - 2006.05.09 Implementato
**					_buildKeysCondition, _buildInsertString
**		v0.1.2 - 2006.05.10
**					Insert
**		v0.1.3 - 2006.05.11
**					Update, _buildUpdateString, Delete, _buildDeleteString
**					Copy
**		v0.1.4 - 2006.05.22 Aggiunto e usato <b>quoteDate</b>
**
** @copyright &copy; 2011 by Roberto Rocco Angeloni <roberto@roccoangeloni.it>
** @license http://opensource.org/licenses/lgpl-3.0.html GNU Lesser General Public License, version 3.0 (LGPLv3)
** @version $Id: dbmgr.h $
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

#ifndef DBMGR_H
#define DBMGR_H

#include "importedPackages.h"

#include "dbconnection.h"
#include "dbentity.h"
#include "dbefactory.h"

namespace DBLayer {

class DECLSPECIFIER DBMgr {
    private:
        Connection* con;
        bool verbose;
        DBEFactory* dbeFactory;
        string _schema;
        DBEntity* _dbeuser;

        string _buildTableName(DBEntity* dbe);
        string _buildKeysCondition(DBEntity* dbe);
        string _buildInsertString(DBEntity* dbe);
        string _buildUpdateString(DBEntity* dbe);
        string _buildDeleteString(DBEntity* dbe);
        string _buildSelectString(DBEntity* dbe, bool uselike=false, bool caseSensitive=true);
        StringVector _buildWhereCondition(DBEntity* dbe, bool uselike=false, bool caseSensitive=true);

    protected:
        string name;
        string errorMessage;

        virtual string escapeString(string s);
        virtual string quoteDate(string s);
        virtual DBEntity* _before_insert(DBEntity* dbe);
        virtual DBEntity* _after_insert(DBEntity* dbe);
        virtual DBEntity* _before_update(DBEntity* dbe);
        virtual DBEntity* _after_update(DBEntity* dbe);
        virtual DBEntity* _before_delete(DBEntity* dbe);
        virtual DBEntity* _after_delete(DBEntity* dbe);
        virtual DBEntity* _before_copy(DBEntity* dbe);
        virtual DBEntity* _after_copy(DBEntity* dbe);

    public:
        static const int MAX_ENTITY = 100;

        DBMgr();
        DBMgr(Connection* con, bool verbose=false);
        virtual ~DBMgr();

        bool connect();
        bool disconnect();

        string getErrorMessage();

        void setDBEFactory(DBEFactory* dbeFactory);
        void setVerbose(bool b);
        DBEFactory* getDBEFactory();
        DBEntityVector getRegisteredTypes();
        /** get clazz by table name */
        DBEntity* getClazz(string* typeName);
        DBEntity* getClazzByTypeName(string* typeName);

        DBEntity* getDBEUser();
        void setDBEUser(DBEntity* dbe);
//        function getUserGroupsList() { return $this->user_groups_list; }
//        function setUserGroupsList($user_groups_list) { $this->user_groups_list = $user_groups_list; }
//        /** The logged user has the requested group_id ? */
//        function hasGroup($group_id) { return in_array(DBEntity::uuid2hex($group_id),$this->user_groups_list); }
//        function addGroup($group_id) { if(!in_array($group_id,$this->user_groups_list)) $this->user_groups_list[]=$group_id; }

        ColumnDefinitions getColumnsForTable(const string& tablename);

//	function getColumnName( $tablename, $num_column ) {
//		$this->connect();
//		if ($this->tipo_dbms=='PG') {
//			return -1;
//		} else if ($this->tipo_dbms=='MYSQL') {
//			$ret="";
//			$result = mysql_query("SHOW COLUMNS FROM $tablename",$this->conn);
//			if ($result===false) {
//				echo 'Could not run query: ' . mysql_error();
//				return $ret;
//			}
//			if (mysql_num_rows($result) > 0) {
//				$colonna=1;
//				while ($row = mysql_fetch_assoc($result)) {
//					if($colonna==$num_column) {
//						$ret=$row["Field"];
//						break;
//					}
//					$colonna++;
//				}
//			}
//			echo "mysql_num_rows(result): ".$ret."\n";
//			return $ret;
//		}
//	}
//	function getColumnSize( $tablename ) {
//		$this->connect();
//		if ($this->tipo_dbms=='PG') {
//			return -1;
//		} else if ($this->tipo_dbms=='MYSQL') {
//			$ret=-1;
//			$result = mysql_query("SHOW COLUMNS FROM $tablename",$this->conn);
//			if ($result===false) {
//				echo 'Could not run query: ' . mysql_error();
//				return $ret;
//			}
//			$ret = mysql_num_rows($result);
//			echo "mysql_num_rows(result): ".$ret."\n";
//			return $ret;
//		}
//	}
//	function getKeys( $tablename ) {
//		$this->connect();
//		if ($this->tipo_dbms=='PG') {
//			return array("TODO");
//		} else if ($this->tipo_dbms=='MYSQL') {
//			$ret=array();
//			$result = mysql_query("SHOW COLUMNS FROM $tablename",$this->conn);
//			if ($result===false) {
//				echo 'Could not run query: ' . mysql_error();
//				return $ret;
//			}
//			if (mysql_num_rows($result) > 0) {
//				$colonna=0;
//				while ($row = mysql_fetch_assoc($result)) {
//					if($row["Key"]=="PRI")
//						$ret[]=$colonna;
//					$colonna++;
//				}
//			}
//			return $ret;
//		}
//	}
//	function getForeignKeys( $tablename ) {
//		$this->connect();
//		if ($this->tipo_dbms=='PG') {
//			return array("TODO");
//		} else if ($this->tipo_dbms=='MYSQL') {
//			$ret=array();
//			$result = mysql_query("SHOW COLUMNS FROM $tablename",$this->conn);
//			if ($result===false) {
//				echo 'Could not run query: ' . mysql_error();
//				return $ret;
//			}
//			if (mysql_num_rows($result) > 0) {
//				$colonna=0;
//				while ($row = mysql_fetch_assoc($result)) {
//					if($row["Key"]=="MUL")
//						$ret[]=$colonna;
//					$colonna++;
//				}
//			}
//			return $ret;
//		}
//	}

        DBEntity* Insert(DBEntity *dbe);
        DBEntity* Update(DBEntity *dbe);
        DBEntity* Delete(DBEntity *dbe);
        DBEntity* Copy(DBEntity *dbe);

        DBEntityVector* Select(const string* tableName, const string* searchString);

        DBEntityVector* Search(DBEntity* dbe, bool uselike=true,
                                                        bool caseSensitive=true, const string* orderBy=new string("") );
        DBEntityVector* searchByKeys(DBEntity* dbe);
        bool exists(DBEntity* dbe);

        /** Distrugge le DBEntity della lista ed anche la lista stessa!	*/
        static void Destroy(DBEntityVector* lista);

        void setConnection( Connection* _newVal);
        Connection* getConnection();

        void setSchema(string schema);
        string getSchema();

//        function getNextId(&$dbe) {
//		$nomeTabella = $this->_schema . "_seq_id";
//		$tmp = $this->_select( get_class( $dbe ), $nomeTabella, "select id from $nomeTabella where name=''" );
//		$myid = count($tmp)>0 ? $tmp[0]->getValue('id') + 1 : 1;
//		if ($this->_verbose) { print "DBMgr.getNextId: nomeTabella=$nomeTabella<br />\n"; }
//		if ($this->_verbose) { print "DBMgr.getNextId: count(tmp)=".count($tmp)."<br />\n"; }
//		if(count($tmp)==0)
//			$this->db_query( "insert into $nomeTabella (id,name) values($myid,'') " );
//		else
//			$this->db_query( "update $nomeTabella set id=$myid where name='' " );
//		return $myid;
//	}
//	/**
//	 * SE esiste uniqid, la usa per generare l'uuid,
//	 * altrimenti usa la vecchia getNextId
//	 * @date 2010.07.27
//	 */
//	function getNextUuid(&$dbe, $prefix="",$length=16) {
//		if(strlen($prefix)==0)
//			$prefix=$_SERVER['SERVER_NAME'];
//		$ret="$prefix.";
//		if(function_exists("uniqid")) {
//			$ret.=uniqid();
//		} else {
//			$ret.=$this->getNextId($dbe);
//		}
//		if(strlen($ret)>$length)
//			$ret=substr($ret,-1*$length);
//		elseif(strlen($ret)<$length)
//			$ret = sprintf("%-16s", $ret);
//		return $ret;
//	}

//        /**
//	 * Date le chiavi di una dbe, controlla se esiste gia una entry nel DB
//	 * @date 2007.07.27
//	 */
//	function exists($dbe) {
//		$ret=false;
//		$myclausole = $this->_buildKeysCondition($dbe);
//		if ( strlen($myclausole)==0 )
//			return $ret;
//		$mytablename = $this->_buildTableName($dbe); // 2009.12.15 $dbe->getTableName();
//		$query = "select count(*) as numero from $mytablename where $myclausole";
//		$classname = $dbe->getTypeName()>"" ? $dbe->getTypeName() : get_class( $dbe );
//		$tmp=$this->select($classname, $mytablename, $query);
//		$ret = $tmp[0]->getValue('numero')>0;
//		return $ret;
//	}
};

}
#endif
