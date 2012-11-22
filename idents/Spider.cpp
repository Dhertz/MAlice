#include "Spider.hpp"

Spider::Spider(boost::shared_ptr<Type> elemType) {
	_elemType = elemType;
	boost::shared_ptr<Array> a(new Array(_elemType));
	_array = a;
}

boost::shared_ptr<Array> Spider::getArray() {
	return _array;
}

boost::shared_ptr<Type> Spider::getElemType() {
	return _elemType;
}

string Spider::getTypeName() {
	return "Array";
}