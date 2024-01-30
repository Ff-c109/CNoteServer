#include <fstream>
#include <iostream>
#include <map>
#include <string>

using namespace std;

namespace note {
	ifstream fin;
	ofstream fout;

	const char* notedb;

	struct note {
		string name;
		string text;
	};
	map<int, note> notes;
	void read(bool open=true) {
		if(open) fin.open(notedb, ios::binary);
		int key;
		if(!fin.read((char*)&key, sizeof(int))) {
			fin.close();
			return;
		}

		int nameSize;
		int textSize;
		note thisNote;

		fin.read((char*)&nameSize, sizeof(int));
		char* buffer = new char[nameSize + 1];
		fin.read(buffer, nameSize);
		buffer[nameSize]=0;
		thisNote.name = buffer;
		delete[] buffer;

		fin.read((char*)&textSize, sizeof(int));
		char* buffer2 = new char[textSize+1];
		fin.read(buffer2, textSize);
		buffer2[textSize]=0;
		thisNote.text = buffer2;
		delete[] buffer2;

		notes[key]=thisNote;
		read(false);
	}
	void write() {
		fout.open(notedb, ios::binary);
		for(const auto& c : notes) {
			fout.write((char*)&c.first, sizeof(int));
			int nameSize = c.second.name.size();
			int textSize = c.second.text.size();
			fout.write((char*)&nameSize, sizeof(int));
			fout.write(c.second.name.c_str(), nameSize);
			fout.write((char*)&textSize, sizeof(int));
			fout.write(c.second.text.c_str(), textSize);
		}
		fout.close();
	}
	string readNote(string dbname, string name) {
		notedb=(dbname + ".dat").c_str();
		read();
		for(auto& c : notes) {
			if(c.second.name == name)
				return c.second.text;
		}
		return "";
	}
	void writeNote(string dbname, string name, string text) {
		notedb=(dbname + ".dat").c_str();
		read();
		bool exist=false;
		for(auto& c : notes) {
			if(c.second.name == name) {
				exist=true;
				c.second.text = text;
			}
		}
		if(!exist) {
			auto p = notes.size();
			notes[p].name = name;
			notes[p].text = text;
		}
		write();
	}
	void deleteNote(string dbname, string name) {
		notedb=(dbname + ".dat").c_str();
		notes.clear();
		read();
		for(auto& c : notes) {
			if(c.second.name == name) {
				notes.erase(c.first);
				break;
			}
		}
		write();
	}

	string listNote(string dbname) {
		notedb = (dbname + ".dat").c_str();
		read();
		string retv = "";
		for(const auto& c : notes) {
			retv += c.second.name + '\n';
		}
		return retv;
	}

	string onListNote(string arg) {
		return listNote(arg);
	}
	string onWriteNote(string arg) {
		string dbname, rest;
		bool split=false;
		bool check=false;
		for(const auto& c : arg) {
			if(c == ' ' && !split) split=true;
			else if(split) rest+=c;
			else dbname += c;
		}
		string name, text;
		split=false;
		for(const auto& c : rest) {
			if(c == ' ' && !split) split=true;
			else if(split) text += c;
			else name += c;
		}
		writeNote(dbname, name, text);
		return "";
	}
	string onReadNote(string arg) {
		string dbname, name;
		bool split=false;
		bool check=false;
		for(const auto& c : arg) {
			if(c == ' ' && !split) split=true;
			else if(split) name+=c;
			else dbname += c;
		}

		return readNote(dbname, name);
	}
	string onDeleteNote(string arg) {
		string dbname, name;
		bool split=false;
		for(const auto& c : arg) {
			if(c == ' ' && !split) split=true;
			else if(split) name += c;
			else dbname += c;
		}
		deleteNote(dbname, name);
		return "";
	}
}

		
		
