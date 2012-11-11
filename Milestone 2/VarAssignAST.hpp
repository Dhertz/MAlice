#ifndef VAR_ASSIGN_AST_H
#define VAR_ASSIGN_AST_H

#include <string>
#include "SymbolTable.hpp"
#include "Identifier.hpp"
#include <iostream>
#include "Variable.hpp"
#include "ExprAST.hpp"

class VarAssignAST {
	SymbolTable* _st;

	string _varName;
	ExprAST _expr;
	Variable _varObj;

public:
	VarAssignAST(SymbolTable* st, string varName, ExprAST expr);
	Type check();

};

#endif
