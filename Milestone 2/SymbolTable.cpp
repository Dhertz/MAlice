#include "SymbolTable.hpp"
#include <iostream>

using namespace std;

SymbolTable::SymbolTable(SymbolTable* st) {
	_encSymTable = st;
}

void SymbolTable::add(string name, Identifier* obj) {
	_dict[name] = obj;
}

Identifier* SymbolTable::lookupCurrLevelOnly(string name) {
	if (_dict.find(name) == _dict.end()) {
		return NULL;
	} else {
		return _dict.at(name);
	}
}

Identifier* SymbolTable::lookupCurrLevelAndEnclosingLevels(string name) {
	SymbolTable* st = this;
	while (st != NULL) {
		Identifier* id = st->lookupCurrLevelOnly(name);
		if (id != NULL) {
			return id;
		}
		st = st->_encSymTable;
	}
	return NULL;
}
