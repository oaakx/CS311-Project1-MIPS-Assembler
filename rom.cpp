#include "rom.h"

ROM::ROM() : pc(0), offset(0) { }
ROM::ROM(int offset) : pc(0), offset(offset) { }
void ROM::addWord(string word) {
	// std::cerr << word << " / " << pc << std::endl;
	words.push_back(word);
	pc++;
}

void ROM::addLabel(const string &label) {
	label2addr[label] = pc;
}

int ROM::getAddr(const string &label) {
	return label2addr[label]*4 + offset;
}