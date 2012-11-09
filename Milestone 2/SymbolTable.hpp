#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <boost/unordered_map.hpp>
#include <string>
#include "Identifier.hpp"

using namespace std;

class SymbolTable {
	SymbolTable* _encSymTable;
	boost::unordered_map<string, Identifier*> _dict;

	public:
		SymbolTable(SymbolTable* st);
		void add(string name, Identifier* obj);
		Identifier* lookupCurrLevelOnly(string name);
		Identifier* lookupCurrLevelAndEnclosingLevels(string name);
};

#endif
