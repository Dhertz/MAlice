#include "Array.hpp"

Array::Array(boost::shared_ptr<Type> elemType) {
    _elemType = elemType;
}

boost::shared_ptr<Type> Array::getElemType() {
    return _elemType;
}

string Array::getTypeName() {
    return "Array";
}
