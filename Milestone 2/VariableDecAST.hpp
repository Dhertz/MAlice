#ifndef VARIABLE_DEC_AST_H
#define VARIABLE_DEC_AST_H

#include "ASTNode.hpp"
#include "Variable.hpp"

class VariableDecAST : public ASTNode {
	SymbolTable* _st;

	string _typeName;
	string _varName;
	Variable* _varObj;

public:
	VariableDecAST(SymbolTable* st, string typeName, string varName);
	void check();
};

#endif
