#ifndef PROG_AST
#define PROG_AST

#include "ASTNode.hpp"
#include "../ASTVisitor.hpp"

class ProgAST : public ASTNode {
	void check();
public:
	ProgAST(boost::shared_ptr<SymbolTable> st,
              boost::weak_ptr<ASTNode> parent, int lineNo);
	void print();
	string getNodeName();
	void accept(ASTVisitor v);
};

#endif
