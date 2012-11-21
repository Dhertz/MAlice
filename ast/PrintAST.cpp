#include "PrintAST.hpp"

PrintAST::PrintAST(boost::shared_ptr<SymbolTable> st, string arrayName, boost::shared_ptr<ExprAST> element) : ASTNode(st) {
	_st = st;
	_name = arrayName;
	_element = element;
}

PrintAST::PrintAST(boost::shared_ptr<SymbolTable> st, string funcName, boost::shared_ptr<CallParamsAST> params) : ASTNode(st) {
	_st = st;
	_name = funcName;
	_params = params;
}

PrintAST::PrintAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr) : ASTNode(st) {
	_st = st;
	_expr = expr;
}

void PrintAST::check() {
	boost::shared_ptr<Identifier> object = _st->lookupCurrLevelAndEnclosingLevels(_name);
	if (!object) {

	} else {
		_type = object;
	}
}

boost::shared_ptr<Identifier> PrintAST::getTypeName() {
	return _type;
}
