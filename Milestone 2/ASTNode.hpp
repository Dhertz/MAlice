#ifndef AST_NODE_H
#define AST_NODE_H

#include "SymbolTable.hpp"

class ASTNode {
protected:
	SymbolTable* _st;
	ASTNode(SymbolTable* st);
public:
	virtual void check() = 0;
};

#endif
