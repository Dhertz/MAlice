#ifndef AST_NODE_H
#define AST_NODE_H

#include "../SymbolTable.hpp"
#include <vector>

class ASTNode {
	vector< boost::shared_ptr<ASTNode> > _children;
protected:
	boost::shared_ptr<SymbolTable> _st;
public:
	ASTNode(boost::shared_ptr<SymbolTable> st);
	vector< boost::shared_ptr<ASTNode> > getChildren();
	void addChild(boost::shared_ptr<ASTNode> child, int position);
	void print();
	virtual void check();
};

#endif
