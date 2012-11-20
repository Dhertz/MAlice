#include "ChoiceAST.hpp"

ChoiceAST::ChoiceAST(SymbolTable* st, ExprAST* expr) : ASTNode(st) {
	_st = st;
	_expr = expr;
	check();
}
 void ChoiceAST::check() {
 	if(_expr->getType()->getID() != "Boolean") {

 	}
 }