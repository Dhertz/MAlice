#include "IfAST.hpp"

IfAST::IfAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr, boost::shared_ptr<ASTNode> parent) : ASTNode(st, parent) {
	_st = st;
	_expr = expr;
}

void IfAST::check() {
	if(!_expr) {
		cerr << "If conditional is not a proper expression." << endl;
	} if(_expr->getTypeName()->getBaseName() != "Boolean") {
		cerr << "If conditional must be of type boolean, is of type" <<
			_expr->getTypeName()->getBaseName() << "." << endl;
	}
}
