#include "IfAST.hpp"

IfAST::IfAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr) : ASTNode(st) {
	_st = st;
	_expr = expr;
}

void IfAST::check() {
	if(_expr->getType()->getID() != "Boolean") {
		
	}
}
