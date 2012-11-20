#include "Array.hpp"

Array::Array(Type* elemType) {
	_elemType = elemType;
}

Type* Array::getElemType() {
	return _elemType;
}

string Array::getID() {
	return "Array";
}


