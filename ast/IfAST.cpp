#include "IfAST.hpp"
#include <iostream>

IfAST::IfAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr, boost::shared_ptr<ASTNode> parent, int lineNo) : ASTNode(st, parent, lineNo) {
	_st = st;
	_expr = expr;
    _lineNo = lineNo;

    check();
}

void IfAST::check() {
	if (!_expr) {
		cerr << "Line " << _lineNo << " - " << "If conditional is not a proper expression." << endl;
	} else if (_expr->getType()->getTypeName() != "Boolean") {
		cerr << "Line " << _lineNo << " - " << "If statement conditional must be of type Boolean, it's of type " <<
			_expr->getType()->getTypeName() << "." << endl;
	}
}
