#ifndef EXPR_AST_H
#define EXPR_AST_H

#include "ASTNode.hpp"
#include <antlr3.h>

class ExprAST : public ASTNode {
public:
	ExprAST(SymbolTable* st, pANTLR3_BASE_TREE tree);
	void check();
};

#endif
