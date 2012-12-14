#include "PullDownAST.hpp"
#include "../Utils.hpp"

PullDownAST::PullDownAST(boost::shared_ptr<SymbolTable> st,
                       boost::shared_ptr<ExprAST> expr, 
					   boost::weak_ptr<ASTNode> parent, int lineNo)
					   : ASTNode(st, parent, lineNo) {
	_st = st;
    _expr = expr;
	_lineNo = lineNo;
	check();
}

void PullDownAST::check() {
    if (!_expr || !_expr->getType()) {
        Utils::printSemErr(_lineNo, (string) "Cannot pull down a bad " +
                             "expression due to earlier error.");
    } else if (_expr->getType()->getTypeName() != "Number") {
        Utils::printSemErr(_lineNo, "Can only pull down a pin Number, not a " +
                             _expr->getType()->getTypeName());
    }
}

void PullDownAST::print() {
	cout << "Pull down GPIO pin" << endl;
}

string PullDownAST::getNodeName() {
	return "PullDown";
}

void PullDownAST::accept(boost::shared_ptr<ASTVisitor> v, 
						 boost::shared_ptr<AssemFunc> func) {
	v->visitPullDown(_st, _expr, func);
}