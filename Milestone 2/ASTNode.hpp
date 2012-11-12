#ifndef AST_NODE_H
#define AST_NODE_H

#include "SymbolTable.hpp"
#include <vector>

class ASTNode {
	vector<ASTNode*> _children;
protected:
	SymbolTable* _st;
	ASTNode(SymbolTable* st);
public:
	vector<ASTNode*> getChildren();
	virtual void check() = 0;
};

#endif
