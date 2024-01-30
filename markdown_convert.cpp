#include <string>
#include <fstream>
#include <cctype>
#include <iostream>
#include <map>

using namespace std;

ifstream fin;
ofstream fout;

namespace markdown_convert{
	int** findAll(string txt, string sb, int now, map<int, int> former) {
		int location = txt.find(sb, now);
		if(location < txt.length()) {
			if(former.empty()) {
				map<int, int> nextFormer;
				nextFormer[0] = location;
				return findAll(txt, sb, location, nextFormer);
			}
			else {
				map<int, int> nextFormer;
				nextFormer = former;
				nextFormer[nextFormer.size()] = location;
				return findAll(txt, sb, location, nextFormer);
			}
		}
		else {
			int* result = new int[former.size()];
			for(int counter = 0; counter < former.size(); counter++) {
				result[counter] = former[counter];
			}
			int** retv = new int*[2];
			retv[0] = result;
			retv[1] = new int;
			*(retv[1]) = former.size();
			return retv;
		}
	}
			
	string reader(string location) {
		fin.open(location.c_str());
		fin.seekg(0, ios::end);
		int length = fin.tellg();
		fin.seekg(0, ios::beg);
		char* buffer = new char[length+2];
		fin.read(buffer, length);
		fin.close();
		string content = buffer;
		delete[] buffer;
		return content;
	}

	string title(string content, int fromWitch) {
		map<int, int> empty;
		int** location = findAll(content, "#", 0, empty);
		string progressive = content;
		for(int i = 0; i < *(location[1]); i++) {
			progressive = progressive.substr(0, i+1);
			progressive += "<h1>";
			auto location2 = progressive.find("\n", i);
			progressive += progressive.substr(i+1, location2-i+1);
			progressive += "</h1>";
			progressive += progressive.substr(location2, progressive.length() - location2);
		}
		return progressive;
	}

	void writer(string path, string txt) {
		fout.open(path.c_str());
		fout << txt << endl;
		fout.close();
	}
			

	string transformer(string location) {
		string file = reader(location);
		string titleTransformed = title(file, 0);
		cout << titleTransformed << endl;
		writer("test.html", titleTransformed);
		return "test.html";
	}
}
