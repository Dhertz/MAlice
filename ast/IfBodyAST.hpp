#ifndef IF_BODY_AST
#define IF_BODY_AST

#include "ASTNode.hpp"
#include "../ASTVisitor.hpp"

class IfBodyAST : public ASTNode {
	void check();
public:
	IfBodyAST(boost::shared_ptr<SymbolTable> st,
                   boost::weak_ptr<ASTNode> parent, int lineNo);
	void print();
	string getNodeName();
	void accept(boost::shared_ptr<ASTVisitor>);
};

#endif