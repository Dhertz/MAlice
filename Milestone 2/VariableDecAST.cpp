#include "VariableDecAST.hpp"

VariableDecAST::VariableDecAST(SymbolTable* st, string typeName, string varName) {
	_st = st;
	_typeName = typeName;
	_varName = varName;
}

Type VariableDecAST::check() {
	Identifier* type = _st->lookupCurrLevelAndEnclosingLevels(_typeName);
	Identifier* name = _st->lookupCurrLevelOnly(_varName);
	if (type == NULL) {
		cout << "unknown type" << _typeName << endl;
		NullType null;
		return null;
	} else if (type->getID()!="Type") {
		cout << _typeName  << "is not a type" << endl;
		NullType null;
		return null;
	} else if (name != NULL) {
		cout << "variable named" << _varName << "already exists!" << endl;
		NullType null;
		return null;
	} else {
		_varObj.setType((Type&) *type);
		_st->add(_varName, &_varObj);
		return (Type&) *type;
	}
}
