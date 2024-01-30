#include <map>
#include <iostream>
#include <fstream>
#include <string>

namespace std::stringprc {
	std::string* split(std::string str, const char& c) {
		bool spl = false;
		std::string f,l;
		for(const auto& d : str) {
			if(d == c) spl = true;
			else if(spl) l += d;
			else f += d;
		}
		string* retv = new std::string[2];
		retv[0] = f;
		retv[1] = l;
		return retv;
	}
}

using namespace std;

namespace auth {
	ifstream fin;
	ofstream fout;
	string dbpath = "authdb.dat";
	struct accinfo {
		string name;
		string passwd;
	};
	map<int, accinfo> authdb;
	bool DBreading() {
		int key;
		if(!fin.read((char*)&key, sizeof(int))) {
			return false;
		}
		int nameSize, passwdSize;
		fin.read((char*)&nameSize, sizeof(int));
		char* name = new char[nameSize+1];
		fin.read(name, nameSize);
		name[nameSize] = 0;
		
		fin.read((char*)&passwdSize, sizeof(int));
		char* passwd = new char[passwdSize+1];
		fin.read(passwd, passwdSize);
		passwd[passwdSize] = 0;
		
		accinfo currentInfo;
		currentInfo.name=name;
		currentInfo.passwd=passwd;

		authdb[key]=currentInfo;

		delete[] name;
		delete[] passwd;
		return true;
	}
	void readDB() {
		fin.open(dbpath.c_str(), ios::binary);
		while (DBreading()) true;
		fin.close();
	}
	void writeDB() {
		fout.open(dbpath.c_str(), ios::binary);
		for(const auto &c : authdb) {
			fout.write((char*)&c.first, sizeof(int));
			const auto &current = c.second;
			int nameSize = current.name.size();
			int passwdSize = current.passwd.size();
			fout.write((char*)&nameSize, sizeof(int));
			fout.write(current.name.c_str(), nameSize);
			fout.write((char*)&passwdSize, sizeof(int));
			fout.write(current.passwd.c_str(), passwdSize);
		}
		fout.close();
	}

	void add(string name, string passwd) {
		fin.open(dbpath.c_str(), ios::binary);
		if(!fin.good()) {
			fin.close();
			authdb[0].name = name;
			authdb[0].passwd = passwd;
			writeDB();
		}
		else {
			fin.close();
			readDB();
			bool exist=false;
			for(auto &c : authdb)  {
				if(c.second.name == name) {
					exist=true;
					c.second.passwd = passwd;
				}
			}
			if(!exist) {
				int p = authdb.size();
				authdb[p].name=name;
				authdb[p].passwd=passwd;
			}
			writeDB();
		}
	}
	
	bool auth(string name, string passwd) {
		readDB();
		for(const auto& c : authdb) {
			if(c.second.name==name)
				if(c.second.passwd==passwd)
					return true;
		}
		return false;
	}
	void remove(string name, string passwd) {
		readDB();
		for(auto& c : authdb) {
			if(c.second.name==name) {
				if(c.second.passwd==passwd) {
					authdb.erase(c.first);
					break;
				}
				else {
					return;
				}
			}
		}
		writeDB();
	}

	string onLogin(string arg) {
		string name, passwd;
		([&] () {
		 auto tmp = stringprc::split(arg, ' ');
		 name = tmp[0];
		 passwd = tmp[1];
		 })();
		if(auth(name, passwd))
			return "true";
		else
			return "false";
	}
	string onSignUp(string arg) {
		string name, passwd;
		([&] () {
		 auto tmp = stringprc::split(arg, ' ');
		 name = tmp[0];
		 passwd = tmp[1];
		 })();

		add(name, passwd);
		return "";
	}
	string onDelUser(string arg) {
		string name, passwd;
		([&] () {
		 auto tmp = stringprc::split(arg, ' ');
		 name = tmp[0];
		 passwd = tmp[1];
		 })();
		remove(name, passwd);
		return "";	
	}
}

