#include "IncAST.hpp"

IncAST::IncAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr) : ASTNode(st) {
	_st = st;
	_expr = expr;
	check();
}


void IncAST::check() {
	if(_expr->getType()->getID() != "Number") {

	}
}
