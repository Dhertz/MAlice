#ifndef VARIABLE_DEC_AST_H
#define VARIABLE_DEC_AST_H

#include "ASTNode.hpp"
#include "ExprAST.hpp"
#include "../idents/Variable.hpp"

class VariableDecAST : public ASTNode {
	SymbolTable* _st;
	string _typeName;
	string _varName;
	ExprAST* _expr;
	Variable* _varObj;

public:
	VariableDecAST(SymbolTable* st, string typeName, string varName);
	VariableDecAST(SymbolTable* st, string typeName, string varName, ExprAST* expr);
	void check();
};

#endif
