#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "idents/Identifier.hpp"

class SymbolTable : public boost::enable_shared_from_this<SymbolTable> {
	boost::shared_ptr<SymbolTable> _encSymTable;
	boost::unordered_map<string, boost::shared_ptr<Identifier> > _dict;

public:
	SymbolTable(boost::shared_ptr<SymbolTable> st);
	void add(string name, boost::shared_ptr<Identifier> obj);
	boost::shared_ptr<Identifier> lookupCurrLevelOnly(string name);
	boost::shared_ptr<Identifier> lookupCurrLevelAndEnclosingLevels(string name);
	void printCurrLevelOnly();
	void printCurrLevelAndEnclosingLevels();
	boost::shared_ptr<SymbolTable> getEncSymTable();
};

#endif
