#ifndef EXPR_AST_H
#define EXPR_AST_H

#include "ASTNode.hpp"
#include "Type.hpp"

class ExprAST : public ASTNode {
	Type* _type;
public:
	ExprAST(SymbolTable* st);
	void check();
	Type* getType();
};

#endif
