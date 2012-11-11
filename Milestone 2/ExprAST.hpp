#ifndef EXPR_AST_H
#define EXPR_AST_H

#include "ASTNode.hpp"

class ExprAST : public ASTNode {
public:
	ExprAST(SymbolTable* st);
	void check();
};

#endif
