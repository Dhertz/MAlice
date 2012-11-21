#include "StdinAST.hpp"

StdinAST::StdinAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr, boost::shared_ptr<ASTNode> parent) : ASTNode(st, parent) {
	_st = st;
	_expr = expr;
	check();
}

void StdinAST::check() {
	if(!_expr) {
		cerr << "Cannot print bad expression." << endl;
	}
}