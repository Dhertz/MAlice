#include "ChoiceAST.hpp"

ChoiceAST::ChoiceAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr) : ASTNode(st) {
	_st = st;
	_expr = expr;
	check();
}

void ChoiceAST::check() {
	if(_expr->getTypeName()->getBaseName() != "Boolean") {

	}
}
