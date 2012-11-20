#include "ArrayDecAST.hpp"

ArrayDecAST::ArrayDecAST(SymbolTable* st, ExprAST* length, string name, string elemType) : ASTNode(st) {
	_st = st;
	_name = name;
	_length = length;
	_elemType = elemType;
	check();
}

void ArrayDecAST::check() {
	Identifier* type = _st->lookupCurrLevelAndEnclosingLevels(_elemType);
	Identifier* name = _st->lookupCurrLevelOnly(_name);
	if (type == NULL) {

	} else if (type->getID() != "Type") {

	} else if (name != NULL) {

	} else if ("Number" != _length->getType()->getID()) {

	} else {
		Array a((Type*) type);
		_arrObj = &a;
		_st->add(_name, _arrObj);
	}
}