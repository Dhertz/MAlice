#ifndef PULL_UP_AST
#define PULL_UP_AST

#include "ASTNode.hpp"
#include "../ASTVisitor.hpp"
#include "../AssemFunc.hpp"
#include "ExprAST.hpp"

class PullUpAST : public ASTNode {
	boost::shared_ptr<SymbolTable> _st;
	int _lineNo;
public:
	PullUpAST(boost::shared_ptr<SymbolTable> st, 
				boost::weak_ptr<ASTNode> parent, int lineNo);
	void check();
	void print();
	string getNodeName();
	void accept(boost::shared_ptr<ASTVisitor> v, 
				  boost::shared_ptr<AssemFunc> func);
};

#endif