#include "formatter.h"

/*
 * R format helpers
 **/
string rFormat(const string &argsStr, const string &opcode, const string &shamt, const string &funct) {
	vector<string> args = getArgs(argsStr);

	string rd = reg2bin(args[0]);
	string rs = reg2bin(args[1]);
	string rt = reg2bin(args[2]);

	string command = opcode + rs + rt + rd + shamt + funct;
	return command;
}

string rShiftFormat(const string &argsStr, const string &opcode, const string &funct) {
	vector<string> args = getArgs(argsStr);

	string rd = reg2bin(args[0]);
	string rs = reg2bin("$0");
	string rt = reg2bin(args[1]);
	string shamt = str2bin(args[2], 5);

	string command = opcode + rs + rt + rd + shamt + funct;
	return command;
}

/*
 * I format helpers
 **/
string iFormat(const string &argsStr, const string &opcode) {
	vector<string> args = getArgs(argsStr);

	string rt = reg2bin(args[0]);
	string rs = reg2bin(args[1]);
	string im = str2bin(args[2]);

	string command = opcode + rs + rt + im;
	return command;
}

string iBranchFormatWithPC(const string &argsStr, const string &opcode, int pc, ROM &rom) {
	vector<string> args = getArgs(argsStr);

	string rs = reg2bin(args[0]);
	string rt = reg2bin(args[1]);
	string target = args[2];

	int diff = rom.getAddr(target) - (rom.offset + pc*4 + 4);
	diff = diff >> 2;
	string im = dec2bin(diff, 16);

	string command = opcode + rs + rt + im;
	return command;
}

string iOffsetFormat(const string &argsStr, const string &opcode) {
	vector<string> args = getArgs(argsStr);

	string rt = reg2bin(args[0]);

	pair<string, string> temp = splitOffsetReg(args[1]);
	string im = temp.first;
	string rs = temp.second;
	
	im = str2bin(im);
	rs = reg2bin(rs);

	string command = opcode + rs + rt + im;
	return command;
}

/*
 * J format helpers
 **/
string jFormat(const string &argsStr, const string &opcode, ROM &rom) {
	vector<string> args = getArgs(argsStr);

	string target = args[0];
	int targetAddr = rom.getAddr(target);
	targetAddr = (0x0fffffff & targetAddr) >> 2;
	string pseudoaddr = dec2bin(targetAddr, 26);

	string command = opcode + pseudoaddr;
	return command;
}
