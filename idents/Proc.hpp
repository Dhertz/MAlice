#ifndef Proc_H
#define Proc_H

#include "Identifier.hpp"
#include "Type.hpp"
#include "Param.hpp"
#include "SymbolTable.hpp"
#include <vector>

using namespace std;

class Proc : public Identifier {
	vector<Param> _params;
	SymbolTable* _st;

public:
	Proc(vector<Param> params, SymbolTable* st);
	string getID();
	vector<Param> getParams();
	SymbolTable getTable();
};

#endif