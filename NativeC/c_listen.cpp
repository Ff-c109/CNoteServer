#include <iostream>
#include <cctype>
#include <string>
#include <fstream>
#include <map>

using namespace std;

namespace auth {
	string onLogin(string arg);
	string onSignUp(string arg);
	string onDelUser(string arg);
}

namespace note {
	string onListNote(string arg);
	string onWriteNote(string arg);
	string onReadNote(string arg);
	string onDeleteNote(string arg);
}

namespace c_listen {
	map<string, void*> funclist;
	void initFunclist();
	void Listen() {
		initFunclist();
		string msg;
		getline(cin, msg);
		if(funclist.find(msg.c_str()) == funclist.end()) {
			Listen();
			return;
		}
		string (*xfunc)(string arg) = (string (*)(string)) funclist[msg.c_str()];
		string arg;
		getline(cin, arg);
		string retv = xfunc(arg);
		cout << retv << endl;
	}

	string echo(string arg) {
		return arg;
	}
	
	void initFunclist() {
		funclist["echo"] = (void*)echo;

		funclist["auth"] = (void*)auth::onLogin;
		funclist["adduser"] = (void*)auth::onSignUp;
		funclist["deluser"] = (void*)auth::onDelUser;

		funclist["listnote"] = (void*)note::onListNote;
		funclist["writenote"] = (void*)note::onWriteNote;
		funclist["readnote"] = (void*)note::onReadNote;
		funclist["deletenote"] = (void*)note::onDeleteNote;
	}
}

