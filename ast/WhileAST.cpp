#include "WhileAST.hpp"

WhileAST::WhileAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr) : ASTNode(st) {
	_st = st;
	_expr = expr;
	check();
}

void WhileAST::check() {
 	if(_expr->getType()->getID() != "Boolean") {

 	}
 }
