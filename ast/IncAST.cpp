#include "IncAST.hpp"

IncAST::IncAST(SymbolTable* st, ExprAST* expr) : ASTNode(st) {
	_st = st;
	_expr = expr;
	check();
}


void IncAST::check() {
	if(_expr->getType()->getID() != "Number") {

	}
}