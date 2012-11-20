#include "WhileAST.hpp"

WhileAST::WhileAST(SymbolTable* st, ExprAST* expr) : ASTNode(st) {
	_st = st;
	_expr = expr;
	check();
}
 void WhileAST::check() {
 	if(_expr->getType()->getID() != "Boolean") {

 	}
 }