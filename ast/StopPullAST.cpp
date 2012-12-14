#include "StopPullAST.hpp"
#include "../Utils.hpp"

StopPullAST::StopPullAST(boost::shared_ptr<SymbolTable> st,
					   boost::shared_ptr<ExprAST> expr,
					   boost::weak_ptr<ASTNode> parent, int lineNo)
					   : ASTNode(st, parent, lineNo) {
	_st = st;
    _expr = expr;
	_lineNo = lineNo;
	check();
}

void StopPullAST::check() {}

void StopPullAST::print() {
	cout << "Pull down GPIO pin" << endl;
}

string StopPullAST::getNodeName() {
	return "StopPull";
}

void StopPullAST::accept(boost::shared_ptr<ASTVisitor> v, 
						 boost::shared_ptr<AssemFunc> func) {
	v->visitStopPull(_st, _expr, func);
}