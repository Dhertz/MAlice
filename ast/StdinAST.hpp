#ifndef STDIN_AST
#define STDIN_AST

#include "ASTNode.hpp"
#include "ExprAST.hpp"

class StdinAST : public ASTNode {
	SymbolTable st;
	ExprAST* _expr;
public:
	StdinAST(SymbolTable* st, ExprAST* expr);
};

#endif