#ifndef DBSHELL_H
#define DBSHELL_H

namespace DBLayer {

class DBShell {
    string connString;

    bool exitLoop;
    map<string,void(*)(DBShell*,string)> commands;
    map<string,string> cmdsHelp;
    string line;
    string cmdOut;

    DBLayer::Connection* con;

  public:
    DBShell(string connString="");
    ~DBShell();

    static void cmdConnect(DBShell* dbShell, string s);
    static void cmdDisconnect(DBShell* dbShell, string s);
    static void cmdExec(DBShell* dbShell, string s);

    static void cmdTable2Cpp(DBShell* dbShell, string s);

    static void cmdHelp(DBShell* dbShell, string s);
    static void cmdQuit(DBShell* dbShell, string s);

    void operator()(bool prompt=true);

};

}

#endif // DBSHELL_H
