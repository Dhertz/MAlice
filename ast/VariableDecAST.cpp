#include "VariableDecAST.hpp"

VariableDecAST::VariableDecAST(SymbolTable* st, string typeName, string varName) : ASTNode(st) {
	_st = st;
	_typeName = typeName;
	_varName = varName;

	check();
}

VariableDecAST::VariableDecAST(SymbolTable* st, string typeName, string varName, ExprAST* expr) : ASTNode(st) {
	_st = st;
	_typeName = typeName;
	_varName = varName;
	_expr = expr;

	check();
}

void VariableDecAST::check() {
	Identifier* type = _st->lookupCurrLevelAndEnclosingLevels(_typeName);
	Identifier* name = _st->lookupCurrLevelOnly(_varName);

	cout << "VariableDecAST::check() is looking for " << _typeName << " in all levels of " << _st << "..." << endl;
	_st->printCurrLevelAndEnclosingLevels();

	if (type == NULL) {
		cerr << "unknown type " << _typeName << endl;
	} else if (type->getID() != "Type") {
		cerr << _typeName  << " is not a type" << endl;
	} else if (name != NULL) {
		cerr << _varName << " is already declared" << endl;
	} else {
		Variable v((Type*) type);
		_varObj = &v;
		_st->add(_varName, _varObj);

		// TODO: if _expr != NULL, assign value too
		if (_expr != NULL) {
			cout << "TODO: assign inline value in VariableDecAST" << endl;
		}
	}
}
