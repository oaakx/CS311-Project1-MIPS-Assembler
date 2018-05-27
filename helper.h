#ifndef HELPER_H_
#define HELPER_H_

#include <iostream>
#include <regex>
#include <string>
#include <cassert>
using namespace std;

#define ddd(x) cerr << #x << " = " << (x) << endl

// Parsing helpers
pair<string, string> getTokens(const string &);
vector<string> getArgs(string);
pair<string, string> splitOffsetReg(const string &);

// Checker helpers
bool is_section(const string &);
bool is_label(const string &);
bool is_register(const string &);
bool is_hex(const string &);

// Basic converter helpers
string dec2bin(int, int = 16);
int hex2dec(const string &);
string hex2bin(const string &, int = 16);

// Register converter helpers
string reg2bin(const string &);

// String converter helpers
int str2dec(const string &);
string str2bin(const string &, int = 16);

#endif