#ifndef VAR_ASSIGN_AST_H
#define VAR_ASSIGN_AST_H

#include "ASTNode.hpp"
#include "ExprAST.hpp"
#include "../objs/Variable.hpp"

class VarAssignAST : public ASTNode {
	SymbolTable* _st;
	string _varName;
	ExprAST* _expr;
	Variable* _varObj;

public:
	VarAssignAST(SymbolTable* st, string varName, ExprAST* expr);
	void check();

};

#endif
