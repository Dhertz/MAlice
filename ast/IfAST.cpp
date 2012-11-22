#include "IfAST.hpp"

IfAST::IfAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr, boost::shared_ptr<ASTNode> parent, int lineNo) : ASTNode(st, parent, lineNo) {
	_st = st;
	_expr = expr;
    _lineNo = lineNo;

    check();
}

void IfAST::check() {
	if(!_expr) {
		cerr << "Line " << _lineNo << " - " << "If conditional is not a proper expression." << endl;
	} if(_expr->getTypeName()->getTypeName() != "Boolean") {
		cerr << "Line " << _lineNo << " - " << "If statement conditional must be of type Boolean, it's of type" <<
			_expr->getTypeName()->getTypeName() << "." << endl;
	}
}
