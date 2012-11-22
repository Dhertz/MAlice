#include "WhileAST.hpp"

WhileAST::WhileAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr, boost::shared_ptr<ASTNode> parent, int lineNo) : ASTNode(st, parent, lineNo) {
	_st = st;
	_expr = expr;
    _lineNo = lineNo;
	check();
}

void WhileAST::check() {
	if (!_expr) {
		cerr << "Line " << _lineNo << " - " << "Invalid conditional in while header." << endl;
	} else if (_expr->getTypeName()->getTypeName() != "Boolean") {
		cerr << "Line " << _lineNo << " - " << "While condition must be a boolean, currently it's a " << _expr->getTypeName()->getBaseName() << "." << endl;
	}
}
