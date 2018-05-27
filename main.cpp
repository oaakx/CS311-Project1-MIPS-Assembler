#include <fstream>

#include "helper.h"
#include "formatter.h"
#include "rom.h"

int main(int argc, char* argv[]){
	char* asmfile;

	if (argc == 2) {
		asmfile = argv[1];
	}
	else {
		printf("invalid input format\n");
		printf("usage: ./runfile <asmfile>\n");
		exit(8);
	}

	string outfile = string(asmfile);

	assert(outfile.length() >= 2);
	assert(outfile[(int)outfile.length() - 2] == '.');
	assert(outfile[(int)outfile.length() - 1] == 's');

	outfile[(int)outfile.length() - 1] = 'o';

	ifstream fin(asmfile);
	ofstream fout(outfile);

	string line;
	string section;

	ROM data(hex2dec("0x10000000"));
	ROM text(hex2dec("0x400000"));
	vector<pair<int, pair<string, string>>> deferred_ops;

	while ( ! fin.eof()) {
		getline(fin, line);

		pair<string, string> temp = getTokens(line);
		string first_token = temp.first;
		string rest_token = temp.second;

		if (is_section(first_token)) {
			section = first_token;
		}
		else if (section == ".data") {
			string rest_of_line;

			if (is_label(first_token)) {
				rest_of_line = rest_token;

				string label = first_token;
				label = label.substr(0, (int)label.length()-1);
				data.addLabel(label);
			}
			else
				rest_of_line = line;

			pair<string, string> temp = getTokens(rest_of_line);
			string first_token2 = temp.first;
			string rest_token2 = temp.second;
			// ddd(rest_of_line);
			// ddd(first_token2);
			// ddd(rest_token2);
			assert(first_token2 == ".word");
			vector<string> args = getArgs(rest_token2);
			string word = str2bin(args[0], 32);
			data.addWord(word);
		}
		else if (section == ".text") {
			string args_str = rest_token;

			// ddd(first_token);

			if (first_token == "addiu") {
				// I format

				string opcode = hex2bin("0x09", 6);

				string command = iFormat(args_str, opcode);
				text.addWord(command);
			}
			else if (first_token == "addu") {
				// R format
				
				string opcode = hex2bin("0x00", 6);
				string shamt = hex2bin("0x00", 5);
				string funct = hex2bin("0x21", 6);

				string command = rFormat(args_str, opcode, shamt, funct);
				text.addWord(command);
			}
			else if (first_token == "and") {
				// R format
				
				string opcode = hex2bin("0x00", 6);
				string shamt = hex2bin("0x00", 5);
				string funct = hex2bin("0x24", 6);

				string command = rFormat(args_str, opcode, shamt, funct);
				text.addWord(command);
			}
			else if (first_token == "andi") {
				// I format

				string opcode = hex2bin("0x0C", 6);

				string command = iFormat(args_str, opcode);
				text.addWord(command);
			}
			else if (first_token == "beq") {
				deferred_ops.push_back( make_pair(text.pc, make_pair(first_token, args_str)) );
				text.addWord("EMPTY");
			}
			else if (first_token == "bne") {
				deferred_ops.push_back( make_pair(text.pc, make_pair(first_token, args_str)) );
				text.addWord("EMPTY");
			}
			else if (first_token == "j") { // SPECIAL
				deferred_ops.push_back( make_pair(text.pc, make_pair(first_token, args_str)) );
				text.addWord("EMPTY");
			}
			else if (first_token == "jal") {
				deferred_ops.push_back( make_pair(text.pc, make_pair(first_token, args_str)) );
				text.addWord("EMPTY");
			}
			else if (first_token == "jr") {
				// R format
				
				string opcode = hex2bin("0x00", 6);
				string shamt = hex2bin("0x00", 5);
				string funct = hex2bin("0x08", 6);

				vector<string> args = getArgs(args_str);

				string rd = reg2bin("$0");
				string rs = reg2bin(args[0]);
				string rt = reg2bin("$0");

				string command = opcode + rs + rt + rd + shamt + funct;
				text.addWord(command);
			}
			else if (first_token == "lui") {
				// I format. SPECIAL: But $rs ignored.

				vector<string> args = getArgs(args_str);

				string opcode = hex2bin("0x0F", 6);
				string rt = reg2bin(args[0]);
				string rs = reg2bin("$0");
				string im = str2bin(args[1]);

				string command = opcode + rs + rt + im;
				text.addWord(command);
			}
			else if (first_token == "lw") {
				// I format. SPECIAL: Syntax: lw $rt, offset($rs).

				string opcode = hex2bin("0x23", 6);

				string command = iOffsetFormat(args_str, opcode);
				text.addWord(command);
			}
			else if (first_token == "la") {
				// SPECIAL: la = lui + ori

				vector<string> args = getArgs(args_str);

				string r = reg2bin(args[0]);
				string target = args[1]; // is label
				string databits = dec2bin(data.getAddr(target), 32);
				string upperbits = databits.substr(0, 16);
				string lowerbits = databits.substr(16, 16);
				// ddd(target);
				// ddd(upperbits);
				// ddd(upperbits);
				// ddd(lowerbits);

				// lui
				string opcode = hex2bin("0x0F", 6);
				string rt = r;
				string rs = reg2bin("$0");
				string im = upperbits;

				string command = opcode + rs + rt + im;
				text.addWord(command);

				// ori
				if (lowerbits != string("0000") + "0000" + "0000" + "0000") {
					string opcode = hex2bin("0x0D", 6);

					string rt = r;
					string rs = r;
					string im = lowerbits;

					string command = opcode + rs + rt + im;
					text.addWord(command);
				}
			}
			else if (first_token == "nor") {
				// R format
				
				string opcode = hex2bin("0x00", 6);
				string shamt = hex2bin("0x00", 5);
				string funct = hex2bin("0x27", 6);

				string command = rFormat(args_str, opcode, shamt, funct);
				text.addWord(command);
			}
			else if (first_token == "or") {
				// R format
				
				string opcode = hex2bin("0x00", 6);
				string shamt = hex2bin("0x00", 5);
				string funct = hex2bin("0x25", 6);

				string command = rFormat(args_str, opcode, shamt, funct);
				text.addWord(command);
			}
			else if (first_token == "ori") {
				// I format

				string opcode = hex2bin("0x0D", 6);

				string command = iFormat(args_str, opcode);
				text.addWord(command);
			}
			else if (first_token == "sltiu") {
				// I format

				string opcode = hex2bin("0x0B", 6);

				string command = iFormat(args_str, opcode);
				text.addWord(command);
			}
			else if (first_token == "sltu") {
				// R format
				
				string opcode = hex2bin("0x00", 6);
				string shamt = hex2bin("0x00", 5);
				string funct = hex2bin("0x2B", 6);

				string command = rFormat(args_str, opcode, shamt, funct);
				text.addWord(command);
			}
			else if (first_token == "sll") {
				// R format. SPECIAL: Syntax: sll $rd, $rt, h
				
				string opcode = hex2bin("0x00", 6);
				string funct = hex2bin("0x00", 6);

				string command = rShiftFormat(args_str, opcode, funct);
				text.addWord(command);
			}
			else if (first_token == "srl") {
				// R format. SPECIAL: Syntax: srl $rd, $rt, h
				
				string opcode = hex2bin("0x00", 6);
				string funct = hex2bin("0x02", 6);

				string command = rShiftFormat(args_str, opcode, funct);
				text.addWord(command);
			}
			else if (first_token == "sw") {
				// SPECIAL: I format. Syntax: sw $rt, offset($rs).

				string opcode = hex2bin("0x2B", 6);

				string command = iOffsetFormat(args_str, opcode);
				text.addWord(command);
			}
			else if (first_token == "subu") {
				// R format
				
				string opcode = hex2bin("0x00", 6);
				string shamt = hex2bin("0x00", 5);
				string funct = hex2bin("0x23", 6);

				string command = rFormat(args_str, opcode, shamt, funct);
				text.addWord(command);
			}
			else if (is_label(first_token)) {
				string label = first_token;
				label = label.substr(0, (int)label.length()-1);
				text.addLabel(label);
			}
		}
	}

	for (int i = 0; i < (int)deferred_ops.size(); ++i) {
		int pc = deferred_ops[i].first;
		string op = deferred_ops[i].second.first;
		string args_str = deferred_ops[i].second.second;

		if (op == "j") {
			string opcode = hex2bin("0x02", 6);
			text.words[pc] = jFormat(args_str, opcode, text);
		}
		else if (op == "jal") {
			string opcode = hex2bin("0x03", 6);
			text.words[pc] = jFormat(args_str, opcode, text);
		}
		else if (op == "beq") {
			string opcode = hex2bin("0x04", 6);
			text.words[pc] = iBranchFormatWithPC(args_str, opcode, pc, text);
		}
		else if (op == "bne") {
			string opcode = hex2bin("0x05", 6);
			text.words[pc] = iBranchFormatWithPC(args_str, opcode, pc, text);
		}
	}

	cout << "\n\n\nREAL OUTPUT\n\n\n";

	cout << dec2bin(text.pc*4, 32) << endl;
	cout << dec2bin(data.pc*4, 32) << endl;

	fout << dec2bin(text.pc*4, 32);
	fout << dec2bin(data.pc*4, 32);

	for (int i = 0; i < (int)text.words.size(); ++i) {
		cout << text.words[i] << endl;
		fout << text.words[i];
	}

	for (int i = 0; i < (int)data.words.size(); ++i) {
		cout << data.words[i] << endl;
		fout << data.words[i];
	}

	fout << endl;

	return 0;
}

