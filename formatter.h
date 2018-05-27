#ifndef FORMATTER_H_
#define FORMATTER_H_

#include <string>
#include <vector>
#include "helper.h"
#include "rom.h"
using namespace std;

// R format
string rFormat(const string &argsStr, const string &opcode, const string &shamt, const string &funct);
string rShiftFormat(const string &argsStr, const string &opcode, const string &funct);

// I format
string iFormat(const string &argsStr, const string &opcode);
string iBranchFormatWithPC(const string &argsStr, const string &opcode, int pc, ROM &rom);
string iOffsetFormat(const string &argsStr, const string &opcode);

// J format
string jFormat(const string &argsStr, const string &opcode, ROM &rom);

#endif