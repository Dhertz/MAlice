#include "IncAST.hpp"

IncAST::IncAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr, boost::shared_ptr<ASTNode> parent) : ASTNode(st, parent) {
	_st = st;
	_expr = expr;
	check();
}


void IncAST::check() {
	if(!_expr){
		cerr << "Cannot increment bad expression." << endl;
	} if(_expr->getTypeName()->getBaseName() != "Number") {
		cerr << "Cannot increment expression of type " 
			<< _expr->getTypeName()->getBaseName() << "." << endl;
	}
}
