#include "PullUpAST.hpp"
#include "../Utils.hpp"

PullUpAST::PullUpAST(boost::shared_ptr<SymbolTable> st, 
					   boost::weak_ptr<ASTNode> parent, int lineNo)
					   : ASTNode(st, parent, lineNo) {
	_st = st;
	_lineNo = lineNo;
	check();
}

void PullUpAST::check() {}

void PullUpAST::print() {
	cout << "Pull up GPIO pin" << endl;
}

string PullUpAST::getNodeName() {
	return "PullUp";
}

void PullUpAST::accept(boost::shared_ptr<ASTVisitor> v, 
						 boost::shared_ptr<AssemFunc> func) {
	v->visitPullUp(_st, func);
}
