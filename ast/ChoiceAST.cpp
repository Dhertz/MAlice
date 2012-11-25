#include "ChoiceAST.hpp"
#include <iostream>

ChoiceAST::ChoiceAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr, boost::shared_ptr<ASTNode> parent, int lineNo) : ASTNode(st, parent, lineNo) {
	_st = st;
	_expr = expr;
	_lineNo = lineNo;
    check();
}

void ChoiceAST::check() {
	if (!_expr || !_expr->getTypeName()) {
		cerr << "Line " << _lineNo << " - " << "Bad choice expression." << endl;
	} else if (_expr->getTypeName()->getTypeName() != "Boolean") {
		cerr << "Line " << _lineNo << " - " << "Conditional must be a boolean, its a " 
			<< _expr->getTypeName()->getTypeName() << "." << endl;
	}
}
