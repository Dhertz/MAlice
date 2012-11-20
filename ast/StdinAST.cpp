#include "StdinAST.hpp"

StdinAST::StdinAST(SymbolTable* st, ExprAST* expr) : ASTNode(st) {
	_st = st;
	_expr = expr;
}
