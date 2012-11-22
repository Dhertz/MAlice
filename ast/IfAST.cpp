#include "IfAST.hpp"

IfAST::IfAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr, boost::shared_ptr<ASTNode> parent) : ASTNode(st, parent) {
	_st = st;
	_expr = expr;

	check();
}

void IfAST::check() {
	if(!_expr) {
		cerr << "If conditional is not a proper expression." << endl;
	} if(_expr->getTypeName()->getTypeName() != "Boolean") {
		cerr << "If statement conditional must be of type Boolean, it's of type" <<
			_expr->getTypeName()->getTypeName() << "." << endl;
	}
}
