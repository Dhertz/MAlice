#ifndef IF_BODY_AST
#define IF_BODY_AST

#include "ASTNode.hpp"

class IfBodyAST : public ASTNode {
public:
	IfBodyAST(boost::shared_ptr<SymbolTable> st,
                   boost::weak_ptr<ASTNode> parent, int lineNo);
	void print();
};

#endif