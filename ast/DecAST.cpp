#include "DecAST.hpp"

DecAST::DecAST(SymbolTable* st, ExprAST* expr) : ASTNode(st) {
	_st = st;
	_expr = expr;
	check();
}

void DecAST::check() {
	if(_expr->getType()->getID() != "Number") {

	}
}