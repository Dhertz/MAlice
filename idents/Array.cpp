#include "Array.hpp"

Array::Array(boost::shared_ptr<Type> elemType) {
	_elemType = elemType;
}

boost::shared_ptr<Type> Array::getElemType() {
	return _elemType;
}

// This doesn't look right...

string Array::getBaseName() {
	return "Array";
}

string Array::getTypeName() {
	return "Array";
}
