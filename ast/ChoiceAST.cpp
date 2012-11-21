#include "ChoiceAST.hpp"

ChoiceAST::ChoiceAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr, boost::shared_ptr<ASTNode> parent) : ASTNode(st, parent) {
	_st = st;
	_expr = expr;
	check();
}

void ChoiceAST::check() {
	if(!_expr) {
		cerr << "Bad choice expression." << endl;
	} else if(_expr->getTypeName()->getBaseName() != "Boolean") {
		cerr << "Conditional must be a boolean, its a " 
			<< _expr->getTypeName()->getBaseName() << "." << endl;
	}
}
