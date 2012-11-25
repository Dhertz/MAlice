#include "DecAST.hpp"
#include <iostream>

DecAST::DecAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr, boost::shared_ptr<ASTNode> parent, int lineNo) : ASTNode(st, parent, lineNo) {
	_st = st;
	_expr = expr;
    _lineNo = lineNo;
	check();
}

void DecAST::check() {
	if (!_expr || !_expr->getTypeName()) {
		cerr << "Line " << _lineNo << " - " << "Bad expression." << endl;
	} else if (_expr->getTypeName()->getTypeName() != "Number") {
		cerr << "Line " << _lineNo << " - " << "Can only decrement a number, not a " 
			<< _expr->getTypeName()->getTypeName() << "!" << endl;
	}
}
