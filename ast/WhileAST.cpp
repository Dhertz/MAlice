#include "WhileAST.hpp"

WhileAST::WhileAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr, boost::shared_ptr<ASTNode> parent) : ASTNode(st, parent) {
	_st = st;
	_expr = expr;
	check();
}

void WhileAST::check() {
 	if(!_expr){
 		cerr << "Invalid conditional in while header." << endl;
 	} else if(_expr->getTypeName()->getTypeName() != "Boolean") {
 		cerr << "While condition must be a boolean, currently it's a " << _expr->getTypeName()->getBaseName() << "." << endl;
 	}
 }
