#ifndef AST_H
#define AST_H

#include "ASTNode.hpp"

class AST {
	boost::shared_ptr<ASTNode> _root;
public:
	void setRoot(boost::shared_ptr<ASTNode> root);
	void print();
	void printNode(boost::shared_ptr<ASTNode> node);
};

#endif
