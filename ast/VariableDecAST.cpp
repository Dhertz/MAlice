#include "VariableDecAST.hpp"

VariableDecAST::VariableDecAST(SymbolTable* st, string typeName, string varName) : ASTNode(st) {
	_st = st;
	_typeName = typeName;
	_varName = varName;
}

VariableDecAST::VariableDecAST(SymbolTable* st, string typeName, string varName, ExprAST* expr) : ASTNode(st) {}

void VariableDecAST::check() {
	Identifier* type = _st->lookupCurrLevelAndEnclosingLevels(_typeName);
	Identifier* name = _st->lookupCurrLevelOnly(_varName);
	if (type == NULL) {
		cout << "unknown type" << _typeName << endl;
		// NullType null;
		// return null;
	} else if (type->getID() != "Type") {
		cout << _typeName  << "is not a type" << endl;
		// NullType null;
		// return null;
	} else if (name != NULL) {
		cout << "variable named" << _varName << "already exists!" << endl;
		// NullType null;
		// return null;
	} else {
		Variable v((Type*) type);
		_varObj = &v;
		_st->add(_varName, _varObj);
	}
}
