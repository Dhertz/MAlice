#ifndef STOP_PULL_AST
#define STOP_PULL_AST

#include "ASTNode.hpp"
#include "../ASTVisitor.hpp"
#include "../AssemFunc.hpp"
#include "ExprAST.hpp"

class StopPullAST : public ASTNode {
	boost::shared_ptr<SymbolTable> _st;
	boost::shared_ptr<ExprAST> _expr;
	int _lineNo;
public:
	StopPullAST(boost::shared_ptr<SymbolTable> st,
				boost::shared_ptr<ExprAST> expr, 
				boost::weak_ptr<ASTNode> parent, int lineNo);
	void check();
	void print();
	string getNodeName();
	void accept(boost::shared_ptr<ASTVisitor> v, 
				  boost::shared_ptr<AssemFunc> func);
};

#endif