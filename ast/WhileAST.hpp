#ifndef WHILE_AST
#define WHILE_AST

#include "ASTNode.hpp"
#include "ExprAST.hpp"

class WhileAST : public ASTNode {
	SymbolTable _st;
	ExprAST* _expr;
public:
	WhileAST(SymbolTable* st, ExprAST* expr);
	void check();
};

#endif