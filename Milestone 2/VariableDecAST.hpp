#ifndef VARIABLE_DEC_AST_H
#define VARIABLE_DEC_AST_H

#include <string>
#include "SymbolTable.hpp"
#include <iostream>
#include "Variable.hpp"
#include "NullType.hpp"

using namespace std;

class VariableDecAST {
	SymbolTable* _st;

	string _typeName;
	string _varName;
	Variable _varObj;

public:
	VariableDecAST(SymbolTable* st, string typeName, string varName);
	Type check();

};

#endif
