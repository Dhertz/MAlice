#include "PullUpAST.hpp"
#include "../Utils.hpp"

PullUpAST::PullUpAST(boost::shared_ptr<SymbolTable> st,
					   boost::shared_ptr<ExprAST> expr, 
					   boost::weak_ptr<ASTNode> parent, int lineNo)
					   : ASTNode(st, parent, lineNo) {
	_st = st;
	_expr = expr;
	_lineNo = lineNo;
	check();
}

void PullUpAST::check() {
	if (!_expr || !_expr->getType()) {
        Utils::printSemErr(_lineNo, (string) "Cannot pull up a bad " +
                             "expression due to earlier error.");
    } else if (_expr->getType()->getTypeName() != "Number") {
        Utils::printSemErr(_lineNo, "Can only pull up a pin Number, not a " +
                             _expr->getType()->getTypeName());
    }
}

void PullUpAST::print() {
	cout << "Pull up GPIO pin" << endl;
}

string PullUpAST::getNodeName() {
	return "PullUp";
}

void PullUpAST::accept(boost::shared_ptr<ASTVisitor> v, 
						 boost::shared_ptr<AssemFunc> func) {
	v->visitPullUp(_st, _expr, func);
}
