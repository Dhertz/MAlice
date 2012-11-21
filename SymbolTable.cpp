#include "SymbolTable.hpp"

using namespace std;

SymbolTable::SymbolTable(boost::shared_ptr<SymbolTable> st) {
	_encSymTable = st;
}

void SymbolTable::add(string name, boost::shared_ptr<Identifier> obj) {
	// cout << "Setting _dict[" << name << "] to " << obj << " in ST " << this << endl;
	_dict[name] = obj;
	// printCurrLevelOnly();	
}

boost::shared_ptr<Identifier> SymbolTable::lookupCurrLevelOnly(string name) {
	// cout << "Looking for " << name << " in " << this << " ..." << endl;
	if (_dict.find(name) == _dict.end()) {
		return boost::shared_ptr<Identifier>();
	} else {
		return _dict.at(name);
	}
}

boost::shared_ptr<Identifier> SymbolTable::lookupCurrLevelAndEnclosingLevels(string name) {
	for (boost::shared_ptr<SymbolTable> st = shared_from_this(); st; st = st->_encSymTable) {
		boost::shared_ptr<Identifier> id = st->lookupCurrLevelOnly(name);
		if (id) {
			return id;
		}
	}	
	return boost::shared_ptr<Identifier>();
}

void SymbolTable::printCurrLevelOnly() {
	cout << "Printing curr level of ST " << this << "..." << endl;
	for (boost::unordered_map<string, boost::shared_ptr<Identifier> >::iterator it = _dict.begin();
	it != _dict.end();
	++it) {
		string name = it->first;
		boost::shared_ptr<Identifier> obj = it->second;
		cout << name << ": " << obj << endl;
	}
	cout << "Done." << endl;
}

void SymbolTable::printCurrLevelAndEnclosingLevels() {
	cout << "Printing all levels of ST " << this << endl;
	for (boost::shared_ptr<SymbolTable> st(this); st != NULL; st = st->_encSymTable) {
		st->printCurrLevelOnly();
	}
}

boost::shared_ptr<SymbolTable> SymbolTable::getEncSymTable() {
	return _encSymTable;
}
