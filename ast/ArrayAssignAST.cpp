#include "ArrayAssignAST.hpp"

ArrayAssignAST::ArrayAssignAST(SymbolTable* st, string name, ExprAST* element, ExprAST* value) : ASTNode(st) {
	_st = st;
	_name = name;
	_element = element;
	_value = value;
	check();
}

void ArrayAssignAST::check() {
	Identifier* array = _st->lookupCurrLevelAndEnclosingLevels(_name);
	
	if(_element->getType()->getID() != "Number") {

	} else if (array == NULL) {

	} else if(array->getID() != "Array") {

	} else if(_value->getType()->getID() == ((Array*) array)->getElemType()->getID()) {
	
	} else {
		_arrObj = (Array*) array;
	}
}
