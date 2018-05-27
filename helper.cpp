#include "helper.h"

/*
 * parsing
 **/
pair<string, string> getTokens(const string &line) {
	regex r(R"(\s*(\S+)(?:\s+(.+$))?)");
	smatch m;
	regex_match(line, m, r);

	string first = m.str(1);
	string rest = m.str(2);

	return make_pair(first, rest);
}

vector<string> getArgs(string argsStr) {
	vector<string> args;

	// TODO: fix this: in `lw $rt, offset($rs)` there can't be space around offset and $rs
	regex r(R"([^\s,]+)");
	smatch m;

	while (regex_search(argsStr, m, r)) {
		string arg = m.str(0);
		args.push_back(arg);
		argsStr = m.suffix().str();
	}

	return args;
}
pair<string, string> splitOffsetReg(const string &offsetReg) {
	regex r(R"(([^\s\(\)]+)\s*\(\s*([^\s\(\)]+)\s*\))");
	smatch m;
	regex_match(offsetReg, m, r);

	string offset = m.str(1);
	string reg = m.str(2);

	return make_pair(offset, reg);
}

/*
 * is it?
 **/
bool is_section(const string &str) {
	return str == ".data" || str == ".text";
}
bool is_label(const string &str) {
	return str.length() > 0 && str.back() == ':';
}
bool is_register(const string &str) {
	return str.length() > 0 && str[0] == '$';
}
bool is_hex(const string &str) {
	return str.length() >= 3 && str[0] == '0' && str[1] == 'x';
}

/*
 * converting
 **/
string dec2bin(int n, int nbits) {
	// DONE: handle negative numbers

	// DONE: set the proper nbits default

	// asserts below dont hold if nbits = 5 and n = 31.
	// so commented them.
	// assert(-(1 << (nbits-1)) <= n);
	// assert(n <= (1 << (nbits-1)) - 1);

	vector<bool> bits;

	// handle the case with negative number carefully
	bool isneg = n < 0;
	if (isneg)
		n += 1 << (nbits - 1);

	do {
		bool lsb = n & 1;
		bits.push_back(lsb);
		n >>= 1;
	} while (n > 0);

	string bin = "";

	for (int i = 0; i < nbits - (int)bits.size(); ++i) {
		if (i == 0 && isneg)
			bin += '1';
		else
			bin += '0';
	}

	for (int i = (int)bits.size() - 1; i >= 0; --i)
		bin += ('0' + bits[i]);

	return bin;
}

int hex2dec(const string &hex) {
	assert(hex.length() >= 3);
	assert(hex[0] == '0');
	assert(hex[1] == 'x');

	int dec = 0;
	for (size_t i = 2; i < hex.length(); ++i) {
		int dig = 0;

		if (isdigit(hex[i]))
			dig = hex[i] - '0';
		else
			dig = tolower(hex[i]) - 'a' + 10;

		dec *= 16;
		dec += dig;
	}

	return dec;
}

string hex2bin(const string &hex, int nbits) {
	return dec2bin(hex2dec(hex), nbits);
}

// registers
string reg2bin(const string &reg) {
	string decstr = reg.substr(1); // omit $ character
	int dec = atoi(decstr.c_str());

	string bin = dec2bin(dec, 5);
	return bin;
}

// decimal or binary as string
int str2dec(const string &str) {
	int dec;

	if (is_hex(str))
		dec = hex2dec(str);
	else
		dec = atoi(str.c_str());

	return dec;
}

string str2bin(const string &str, int nbits) {
	int dec = str2dec(str);
	string bin = dec2bin(dec, nbits);

	return bin;
}