#include "Array.hpp"

Array::Array(boost::shared_ptr<Type> elemType) {
    _elemType = elemType;
	_assLoc = "";
}

boost::shared_ptr<Type> Array::getElemType() {
    return _elemType;
}

string Array::getTypeName() {
    return "Array";
}

string Array::getAssLoc() {
	return _assLoc;
}

void Array::setAssLoc(string assLoc) {
	_assLoc = assLoc;
}
