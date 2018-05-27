#ifndef ROM_H_
#define ROM_H_

#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;

// screw access specifiers for now :/

class ROM {
public:
	vector<string> words;
	int pc;
	int offset;
	map<string, int> label2addr;

	ROM();
	ROM(int);
	void addWord(string);
	void addLabel(const string &);
	int getAddr(const string &);
};

#endif