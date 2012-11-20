#include "SymbolTable.hpp"

using namespace std;

SymbolTable::SymbolTable() {
	_encSymTable = NULL;
}

SymbolTable::SymbolTable(SymbolTable* st) {
	_encSymTable = st;
}

void SymbolTable::add(string name, Identifier* obj) {
	cout << "Setting _dict[" << name << "] to " << obj << " in ST " << this << endl;
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
	for (SymbolTable* st = this; st != NULL; st = st->_encSymTable) {
		Identifier* id = st->lookupCurrLevelOnly(name);
		if (id != NULL) {
			return id;
		}
	}
	return NULL;
}

void SymbolTable::printCurrLevelOnly() {
	cout << "Printing curr level of ST " << this << "..." << endl;
	for (boost::unordered_map<string, Identifier*>::iterator it = _dict.begin();
	it != _dict.end();
	++it) {
		string name = it->first;
		Identifier* obj = it->second;
		cout << name << ": " << obj << endl;
	}
	cout << "Done." << endl;
}

void SymbolTable::printCurrLevelAndEnclosingLevels() {
	cout << "Printing all levels of ST " << this << endl;
	for (SymbolTable* st = this; st != NULL; st = st->_encSymTable) {
		st->printCurrLevelOnly();
	}
}

SymbolTable* SymbolTable::getEncSymTable() {
	return _encSymTable;
}
