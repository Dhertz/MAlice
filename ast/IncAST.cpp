#include "IncAST.hpp"
#include <iostream>

IncAST::IncAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr, boost::shared_ptr<ASTNode> parent, int lineNo) : ASTNode(st, parent, lineNo) {
	_st = st;
	_expr = expr;
    _lineNo = lineNo;
	check();
}

void IncAST::check() {
	if (!_expr) {
		cerr << "Line " << _lineNo << " - " << "Cannot increment bad expression." << endl;
	} else if (_expr->getTypeName()->getTypeName() != "Number") {
		cerr << "Line " << _lineNo << " - " << "Cannot increment expression of type " 
			<< _expr->getTypeName()->getTypeName() << "." << endl;
	}
}
