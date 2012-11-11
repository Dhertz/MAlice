#ifndef FUNCTION_H
#define FUNCTION_H

#include "Identifier.hpp"
#include "Type.hpp"
#include "Param.hpp"
#include "SymbolTable.hpp"
#include <vector>

using namespace std;

class Function : public Identifier {
	Type _returnType;
	vector<Param> _params;
	SymbolTable* _st;

public:
	Function(Type returnType, vector<Param> params, SymbolTable* st);
	Type getType();
	vector<Param> getParams();
	SymbolTable getTable();
};

#endif
