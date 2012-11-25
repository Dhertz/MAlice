#include "StdinAST.hpp"
#include <iostream>

StdinAST::StdinAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr, boost::shared_ptr<ASTNode> parent, int lineNo) : ASTNode(st, parent, lineNo) {
	_st = st;
	_expr = expr;
    _lineNo = lineNo;
	check();
}

void StdinAST::check() {
	if (!_expr) {
		cerr << "Line " << _lineNo << " - " << "Stdin has bad expression." << endl;
	} else if (!_expr->isDeclarable()) {
		cerr << "Line " << _lineNo << " - " 
			<< "Stdin can only write to variables or arrays, not " 
				<< _expr->getTypeName()->getTypeName() << "." << endl;
	}
}
