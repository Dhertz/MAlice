#ifndef AST_H
#define AST_H

#include "ASTNode.hpp"

class AST {
	ASTNode* _root;
public:
	void setRoot(ASTNode* root);
};

#endif
