#include "IfAST.hpp"

IfAST::IfAST(SymbolTable* st, ExprAST* expr) : ASTNode(st) {
	_st = st;
	_expr = expr;
}

void IfAST::check() {
	if(_expr->getType()->getID() != "Boolean") {
		
	}
}
