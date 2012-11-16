#ifndef EXPR_AST_H
#define EXPR_AST_H

#include <antlr3.h>
#include "ASTNode.hpp"
#include "Type.hpp"

class ExprAST : public ASTNode {
	Type* _type;
public:
	ExprAST(SymbolTable* st, pANTLR3_BASE_TREE tree);
	void check();
	Type* getType();
};

#endif
