#include "dblayer/dbconnection.h"
#include "dblayer/dbmgr.h"
using namespace DBLayer;

#include "dbshell.h"

DBShell::DBShell(string connString): connString(connString), exitLoop(false),con(0) {
    this->commands["connect"]=&cmdConnect;    this->cmdsHelp["connect"]=" Syntax: connect [conn.string]\n Connects to the desired DB";
    this->commands["disconnect"]=&cmdDisconnect;    this->cmdsHelp["disconnect"]=" Syntax: disconnect\n Disconnect from db";
    this->commands["exec"]=&cmdExec;    this->cmdsHelp["exec"]=" Syntax: exec sql\n Exec an sql statement";

    this->commands["table2cpp"]=&cmdTable2Cpp;    this->cmdsHelp["table2cpp"]=" Syntax: table2cpp table1 [table2] ...\n Generate code for the tables in the list.";

    this->commands["help"]=&cmdHelp;    this->cmdsHelp["help"]=" Syntax: help\n Show available commands";
    this->commands["quit"]=&cmdQuit;    this->cmdsHelp["quit"]=" Syntax: quit\n Quit the shell";

//    this->commands["xxx"]=&cmdXxx;    this->cmdsHelp["xxx"]="Syntax: xxx arg1\n Descrip.";
}
DBShell::~DBShell() {
    if(this->con!=0) delete con;
}

void DBShell::cmdConnect(DBShell* dbShell, string s) {
    dbShell->cmdDisconnect(dbShell,"");
    if(s.length()>0) dbShell->connString=s;
    dbShell->con = DBLayer::createConnection( dbShell->connString.c_str() );
    dbShell->con->connect();
    if ( !dbShell->con->hasErrors() ) {
        dbShell->cmdOut = "Connected to: " + dbShell->connString;
    } else {
        dbShell->cmdOut = "Connection Errors: " + dbShell->con->getErrorMessage();
    }
}
void DBShell::cmdDisconnect(DBShell* dbShell, string s) {
    if(dbShell->con==0) return;
    dbShell->con->disconnect();
    delete dbShell->con;
    dbShell->con=0;

    dbShell->cmdOut = "Disconnected from: " + dbShell->connString;
}
void DBShell::cmdExec(DBShell* dbShell, string s) {
    if(dbShell->con==0 || !dbShell->con->isConnected()) {
        dbShell->cmdOut = "Not connected!";
        return;
    }
    DBLayer::ResultSet* res;
    res = dbShell->con->exec(s);
    if( !dbShell->con->hasErrors() ) {
        dbShell->cmdOut.append("res.status: ").append(res->getStatus()).append("\n");
        dbShell->cmdOut.append("res.toString() = ").append(res->toString()).append("\n");

        int nColonne = res->getNumColumns();
        dbShell->cmdOut.append("N. Colonne: ").append(DBLayer::integer2string(nColonne)).append("\n");
        for( int i=0; i<nColonne; i++) {
            dbShell->cmdOut.append("Colonna[").append(DBLayer::integer2string(i)).append("]: ").append(res->getColumnName(i))
                 .append(" - ").append(res->getColumnType(i))
                 .append(": ").append(DBLayer::integer2string(res->getColumnSize(i)))
                 .append("\n");
        }

        int nRighe = res->getNumRows();
        dbShell->cmdOut.append(" Righe:").append(DBLayer::integer2string(nRighe)).append("\n");
        for(int r=0; r<nRighe; r++) {
            for(int c=0; c<nColonne; c++) {
                if (! res->isNull(r,c) ) {
                    dbShell->cmdOut.append(res->getValue(r,c)).append("\t");
                } else {
                    dbShell->cmdOut.append("\\N").append("\t");
                }
            }
            dbShell->cmdOut.append("\n");
        }
    } else {
        dbShell->cmdOut = "Error executing \"" + s + "\": " + dbShell->con->getErrorMessage();
    }
    delete res;
}

void DBShell::cmdTable2Cpp(DBShell* dbShell, string s) {
    if(dbShell->con==0 || !dbShell->con->isConnected()) {
        dbShell->cmdOut = "Not connected!";
        return;
    }

    string mySchemaName("MySchema");
    string dbSchemaName;
    string schemaSeparator("_");

    dbShell->cmdOut.append("Table(s): ").append(s).append("\n");
    dbShell->cmdOut.append("\n");

    StringVector tables;
    DBLayer::splitString(s,' ',tables);

    dbShell->cmdOut.append("/**\n");
    dbShell->cmdOut.append(" * dbschema.h\n");
    dbShell->cmdOut.append(" */\n");
    dbShell->cmdOut.append("\n");
    dbShell->cmdOut.append("#ifndef ").append(DBLayer::upperCase(mySchemaName)).append("_H\n");
    dbShell->cmdOut.append("#define ").append(DBLayer::upperCase(mySchemaName)).append("_H\n");
    dbShell->cmdOut.append("\n");
    dbShell->cmdOut.append("#include \"dblayer/dblayer.h\"\n");
    dbShell->cmdOut.append("#include \"dblayer/dbentity.h\"\n");
    dbShell->cmdOut.append("#include \"dblayer/dbfield.h\"\n");
    dbShell->cmdOut.append("#include \"dblayer/dbmgr.h\"\n");
    dbShell->cmdOut.append("using namespace DBLayer;\n");
    dbShell->cmdOut.append("#include <string>\n");
    dbShell->cmdOut.append("using namespace std;\n)");
    dbShell->cmdOut.append("\n");
    dbShell->cmdOut.append("namespace ").append(mySchemaName).append(" {\n");
    dbShell->cmdOut.append("\n");

    for(StringVector::const_iterator it=tables.begin(); it!=tables.end(); it++) {
        string fullTableName=(*it);
        string tableName=(*it);
        string schemaName;
        unsigned long schemaSeparator_index = tableName.find_first_of(schemaSeparator);
        if(schemaSeparator_index>0) {
            schemaName = tableName.substr(0,schemaSeparator_index);
            tableName = tableName.substr(schemaSeparator_index+schemaSeparator.length());

            if(dbSchemaName.length()==0) dbSchemaName = schemaName;
        }
        string typeName("DBE"); typeName.append(DBLayer::capitalizeCase(tableName));
        string myquery; myquery.append("select * from ").append((*it)).append(" where 1=0");
//        dbShell->cmdOut.append("tableName:\t").append(tableName).append("\n");
//        dbShell->cmdOut.append("typeName:\t").append(typeName).append("\n");
//        dbShell->cmdOut.append("myquery:\t").append(myquery).append("\n");
        DBLayer::ResultSet* res;
        res = dbShell->con->exec(myquery);
        if( !dbShell->con->hasErrors() ) {
            dbShell->cmdOut.append("  class ").append(typeName).append(" : public DBEntity {\n");
            dbShell->cmdOut.append("    public:\n");
            dbShell->cmdOut.append("    ").append(typeName).append("();\n");
            dbShell->cmdOut.append("      virtual ~").append(typeName).append("();\n");
            dbShell->cmdOut.append("      virtual string name();\n");
            if(schemaName.length()>0) dbShell->cmdOut.append("      virtual const string* getSchemaName();\n");
            dbShell->cmdOut.append("      virtual string getTableName();\n");
            dbShell->cmdOut.append("      virtual DBFieldVector* getKeys();\n");
            dbShell->cmdOut.append("      virtual ").append(typeName).append("* createNewInstance();\n");
            dbShell->cmdOut.append("    private:\n");
            dbShell->cmdOut.append("      static const string nomiCampiChiave[];\n");
            DBLayer::IntegerVector chiavi = dbShell->con->getKeys( &fullTableName );
            if( !dbShell->con->hasErrors() ) {
                unsigned int nChiavi = (unsigned int) chiavi.size();
                int numChiave=1;
                for(unsigned int i=0; i<nChiavi; i++) {
                    string tipoField("StringField");
                    string tipo_chiave = res->getColumnType( chiavi[i] );
                    if(tipo_chiave=="integer") tipoField="IntegerField";
                    dbShell->cmdOut.append("      static ").append(tipoField).append(" chiave").append(DBLayer::integer2string(numChiave)).append("; // ").append(tipo_chiave).append("\n");
                    //res->getColumnName( chiavi[i] );
                    numChiave++;
                }
            } else {
                dbShell->cmdOut.append("// Error while getKeys(): ").append(dbShell->con->getErrorMessage()).append("\n");
            }
            dbShell->cmdOut.append("      static DBFieldVector chiavi;\n");
            dbShell->cmdOut.append("      static DBFieldVector ___init_keys();\n");
            dbShell->cmdOut.append("  };\n");
            dbShell->cmdOut.append("\n");
        } else {
            dbShell->cmdOut = "Error executing \"" + s + "\": " + dbShell->con->getErrorMessage();
        }
        delete res;
    }
    dbShell->cmdOut.append("  string getSchema();\n");
    dbShell->cmdOut.append("  void registerClasses(DBEFactory* dbeFactory);\n");
    dbShell->cmdOut.append("}\n");
    dbShell->cmdOut.append("\n");
    dbShell->cmdOut.append("#endif\n");
    dbShell->cmdOut.append("\n");

    dbShell->cmdOut.append("\n");
    dbShell->cmdOut.append("\n");

    dbShell->cmdOut.append("/**\n");
    dbShell->cmdOut.append(" * dbschema.cpp\n");
    dbShell->cmdOut.append(" */\n");
    dbShell->cmdOut.append("\n");
    dbShell->cmdOut.append("#include \"dbschema.h\"\n");
    dbShell->cmdOut.append("using namespace ").append(mySchemaName).append(";\n");
    dbShell->cmdOut.append("\n");
    dbShell->cmdOut.append("#include <string>\n");
    dbShell->cmdOut.append("using namespace std;\n");
    dbShell->cmdOut.append("\n");
    StringVector myRegisterClasses;
    for(StringVector::const_iterator it=tables.begin(); it!=tables.end(); it++) {
        string fullTableName=(*it);
        string tableName=(*it);
        string schemaName;
        unsigned long schemaSeparator_index = tableName.find_first_of(schemaSeparator);
        if(schemaSeparator_index>0) {
            schemaName = tableName.substr(0,schemaSeparator_index);
            tableName = tableName.substr(schemaSeparator_index+schemaSeparator.length());

            if(dbSchemaName.length()==0) dbSchemaName = schemaName;
        }
        string typeName("DBE"); typeName.append(DBLayer::capitalizeCase(tableName));
        string myquery; myquery.append("select * from ").append((*it)).append(" where 1=0");
        DBLayer::ResultSet* res;
        res = dbShell->con->exec(myquery);
        if( !dbShell->con->hasErrors() ) {
            dbShell->cmdOut.append("//*********************** ").append(typeName).append(": start.\n");
            DBLayer::IntegerVector chiavi = dbShell->con->getKeys( &fullTableName );
            StringVector lista_nomi_chiavi = dbShell->con->getKeysNames( &fullTableName );
            dbShell->cmdOut.append("const string ").append(typeName).append("::nomiCampiChiave[] = { string(\"")
                    .append( DBLayer::joinString(&lista_nomi_chiavi,&string("\"), string(\"")) )
                    .append( "\") };\n" );

            int num_chiave=0;
            StringVector append_chiavi;
            unsigned int nChiavi = (unsigned int) chiavi.size();
            for(unsigned int i=0; i<nChiavi; i++) {
                string tipoField("StringField");
                string tipo_chiave = res->getColumnType( chiavi[i] );
                if(tipo_chiave=="integer") tipoField="IntegerField";
                dbShell->cmdOut
                        .append(tipoField)
                        .append(" ").append(typeName).append("::chiave").append(DBLayer::integer2string(num_chiave+1))
                        .append("( (const string*)&")
                        .append(typeName)
                        .append("::nomiCampiChiave[")
                        .append(DBLayer::integer2string(num_chiave))
                        .append("] );\n");
                //print "$tipoField $typeName::chiave".($num_chiave+1)."( (const string*)&$typeName::nomiCampiChiave[$num_chiave] );\n";
                //res->getColumnName( chiavi[i] );
                append_chiavi.push_back( string("ret.push_back( &").append(typeName).append("::chiave").append(DBLayer::integer2string(num_chiave+1)).append(" );") );
                num_chiave++;
            }

            dbShell->cmdOut.append("DBFieldVector ").append(typeName).append("::chiavi = ").append(typeName).append("::___init_keys();\n");
            dbShell->cmdOut.append("DBFieldVector ").append(typeName).append("::___init_keys() {");
            dbShell->cmdOut.append(" DBFieldVector ret;"); //ret = DBFieldVector();");
            dbShell->cmdOut.append(" ").append(DBLayer::joinString(&append_chiavi,&string(" ")));
            dbShell->cmdOut.append(" return ret; }\n");
            dbShell->cmdOut.append("").append(typeName).append("::").append(typeName).append("() { this->tableName.clear(); }\n");
            dbShell->cmdOut.append("").append(typeName).append("::~").append(typeName).append("() {}\n");
            dbShell->cmdOut.append("string ").append(typeName).append("::name() { return \"").append(typeName).append("\"; }\n");
            dbShell->cmdOut.append("string ").append(typeName).append("::getTableName() { return \"").append(tableName).append("\"; }\n");

            if(schemaName.length()>0)
                    dbShell->cmdOut.append("const string* ").append(typeName).append("::getSchemaName() { static const string __myschema=\"")
                            .append(schemaName)
                            .append("\"; return &__myschema; }\n");
            dbShell->cmdOut.append("DBFieldVector* ").append(typeName).append("::getKeys() { return &").append(typeName).append("::chiavi; }\n");
            dbShell->cmdOut.append("").append(typeName).append("* ").append(typeName).append("::createNewInstance() { return new ").append(typeName).append("(); }\n");
            dbShell->cmdOut.append("//*********************** ").append(typeName).append(": end.\n");
            dbShell->cmdOut.append("\n");
            myRegisterClasses.push_back(
                    string("dbeFactory->registerClass(\"").append(tableName).append("\", new ").append(typeName).append("() );")
                );

        } else {
            dbShell->cmdOut = "Error executing \"" + s + "\": " + dbShell->con->getErrorMessage();
        }
        delete res;
    }
    dbShell->cmdOut.append("string ").append(mySchemaName).append("::getSchema() { return \"").append(dbSchemaName).append("\"; }\n");
    dbShell->cmdOut.append("void ").append(mySchemaName).append("::registerClasses(DBEFactory* dbeFactory) {\n");
    dbShell->cmdOut.append("  ").append(DBLayer::joinString(&myRegisterClasses,&string("\n  "))).append("\n");
    dbShell->cmdOut.append("}\n");
    dbShell->cmdOut.append("\n");

    dbShell->cmdOut.append("table2cpp: TODO - ForeignKeys e altro\n");
}
/*
connect dblayer:sqlite:./examples/test.db
connect dblayer:mysql:host=localhost;dbname=rproject;user=root;password=;
connect dblayer:pg:host=localhost dbname=roberto user=roberto password=roberto
table2cpp rra_users rra_people rra_timetracks rra_todo mylog
table2cpp rra_users mylog
disconnect
quit


connect dblayer:mysql:host=localhost;dbname=namirialpec;user=root;password=;
table2cpp ADMIN ADMIN_STORY AZIENDE COS DOMINI DOMINI_STORY STATO STATO_ADMIN STATO_DOMINI TITOLARI TITOLARI_TMP UTENTI UTENTI_STORY
table2cpp UTENTI_STORY UTENTI
disconnect
quit
*/

void DBShell::cmdHelp(DBShell* dbShell, string s) {
    dbShell->cmdOut.append("DBShell\n=======\n\n");
    dbShell->cmdOut.append("Available commands:\n\n");
    for(map<string,string>::const_iterator it=dbShell->cmdsHelp.begin(); it!=dbShell->cmdsHelp.end(); it++) {
        dbShell->cmdOut
                .append("- ").append((*it).first).append("\n")
                .append( (*it).second ).append("\n\n");
    }
}
void DBShell::cmdQuit(DBShell* dbShell, string s) {
    dbShell->exitLoop=true;
    dbShell->cmdOut = "Bye!";
}

void DBShell::operator()(bool prompt) {
    cout << "- Type quit to exit or help for a list of commands." << endl;
    while(!exitLoop) { // && cin>>line) {
        if(prompt) cout << "> ";
        getline(cin, line);
        int index = line.find(" ");
        string cmd = line.substr(0,index);
        string args = index<0 ? "" : line.substr(index+1);
        map<string,void(*)(DBShell*,string)>::const_iterator mycmd = this->commands.find(cmd);
        this->cmdOut.clear();
        if(mycmd!=this->commands.end()) {
            (*mycmd).second(this,args);
        } else {
            if(this->con!=0 && this->con->isConnected()) {
                cmdExec(this,line);
            } else
                this->cmdOut.append("Unknown command: ").append(cmd).append("(").append(args).append(")");
        }
        if(this->cmdOut.length()>0) cout << (prompt?"- ":"") << this->cmdOut << endl;
    }
}
