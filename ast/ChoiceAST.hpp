#ifndef CHOICE_AST
#define CHOICE_AST

#include "ASTNode.hpp"
#include "ExprAST.hpp"

class ChoiceAST : public ASTNode {
	SymbolTable _st;
	ExprAST* _expr;
public:
	ChoiceAST(SymbolTable* st, ExprAST* expr);
	void check();
};

#endif