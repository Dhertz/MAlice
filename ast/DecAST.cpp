#include "DecAST.hpp"

DecAST::DecAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr) : ASTNode(st) {
	_st = st;
	_expr = expr;
	check();
}

void DecAST::check() {
	if (!_expr->getTypeName()) {
		cerr << "Bad expression." << endl;
	} else if(_expr->getTypeName()->getBaseName() != "Number") {
		cerr << "Can only decrement a number, not a " 
			<< _expr->getTypeName()->getBaseName() << "!" << endl;
	}
}
