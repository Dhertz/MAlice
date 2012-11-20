#include "ReturnAST.hpp"

ReturnAST::ReturnAST(SymbolTable* st, ExprAST* expr) : ASTNode(st) {
	_st = st;
	_expr = expr;
}
