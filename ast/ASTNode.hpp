#ifndef AST_NODE_H
#define AST_NODE_H

#include "../SymbolTable.hpp"
#include <vector>

class ASTNode {
	vector< boost::shared_ptr<ASTNode> > _children;
protected:
	boost::shared_ptr<SymbolTable> _st;
	boost::shared_ptr<ASTNode> _parent;
public:
	ASTNode(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ASTNode> parent);
	vector< boost::shared_ptr<ASTNode> > getChildren();
	void addChild(boost::shared_ptr<ASTNode> child, int position);
    boost::shared_ptr<ASTNode> getParent();
	void print();
	virtual string getNodeName();
	virtual void check();
};

#endif
