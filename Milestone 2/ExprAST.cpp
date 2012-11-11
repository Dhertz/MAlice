#include "ExprAST.hpp"

ExprAST::ExprAST(SymbolTable* st) : ASTNode(st) {
	
}

void ExprAST::check() {
	_st->add("lol", NULL);
}
