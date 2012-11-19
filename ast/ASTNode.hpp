#ifndef AST_NODE_H
#define AST_NODE_H

#include "../SymbolTable.hpp"
#include <vector>

class ASTNode {
	vector<ASTNode*> _children;
protected:
	boost::shared_ptr<SymbolTable> _st;
public:
	ASTNode(boost::shared_ptr<SymbolTable> st);
	vector<ASTNode*> getChildren();
	void addChild(ASTNode* child, int position);
	void print();
	virtual void check();
};

#endif
