#ifndef IF_AST
#define IF_AST

#include "ASTNode.hpp"
#include "ExprAST.hpp"

class IfAST : public ASTNode {
	SymbolTable* _st;
	ExprAST* _expr;
public:
	IfAST(SymbolTable* st, ExprAST* expr);
	void check();
};

#endif