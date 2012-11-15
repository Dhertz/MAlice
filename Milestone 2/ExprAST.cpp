#include "ExprAST.hpp"

ExprAST::ExprAST(SymbolTable* st, pANTLR3_BASE_TREE tree) : ASTNode(st) {
	
}

void ExprAST::check() {
	_st->add("lol", NULL);
}
