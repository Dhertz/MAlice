#ifndef EXPR_AST_H
#define EXPR_AST_H

#include <antlr3.h>
#include "ASTNode.hpp"
#include "../idents/Type.hpp"

class ExprAST : public ASTNode {
	Type* _type;
	pANTLR3_BASE_TREE _tree;
	pANTLR3_BASE_TREE childByNum(pANTLR3_BASE_TREE tree, int num);
	string createStringFromTree(pANTLR3_BASE_TREE tree);
public:
	ExprAST(SymbolTable* st, pANTLR3_BASE_TREE tree);
	void check();
	Type* getType();
};

#endif
