#include "PrintAST.hpp"

PrintAST::PrintAST(SymbolTable* st, string name, ExprAST* element) : ASTNode(st) {
	_st = st;
	_name = name;
	_element = element;
}

PrintAST::PrintAST(SymbolTable* st, string name, CallParamsAST* params) : ASTNode(st) {
	_st = st;
	_name = name;
	_params = params;
}

PrintAST::PrintAST(SymbolTable* st, ExprAST* expr) : ASTNode(st) {
	_st = st;
	_expr = expr;
}

void PrintAST::check() {
	Identifier* object = _st->lookupCurrLevelAndEnclosingLevels(_name);
	if (object == NULL) {

	} else {
		_type = object;
	}
}

Identifier* PrintAST::getType() {
	return _type;
}