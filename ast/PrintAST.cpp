#include "PrintAST.hpp"

PrintAST::PrintAST(boost::shared_ptr<SymbolTable> st, string arrayName, boost::shared_ptr<ExprAST> element, boost::shared_ptr<ASTNode> parent) : ASTNode(st, parent) {
	_st = st;
	_name = arrayName;
	_element = element;
}

PrintAST::PrintAST(boost::shared_ptr<SymbolTable> st, string funcName, boost::shared_ptr<CallParamsAST> params, boost::shared_ptr<ASTNode> parent) : ASTNode(st, parent) {
	_st = st;
	_name = funcName;
	_params = params;
}

PrintAST::PrintAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr, boost::shared_ptr<ASTNode> parent) : ASTNode(st, parent) {
	_st = st;
	_expr = expr;
}

void PrintAST::check() {
	boost::shared_ptr<Identifier> object = _st->lookupCurrLevelAndEnclosingLevels(_name);
	if (!object) {
		cerr << "Cannot print " << _name << " as it doesn't exist." << endl;
	} else {
		_type = object;
	}
}

boost::shared_ptr<Identifier> PrintAST::getTypeName() {
	return _type;
}
