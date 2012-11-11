#include "Param.hpp"

Param::Param(Type* type) {
	_type = type;
}

Type* Param::getType() {
	return _type;
}

string Param::getID() {
	return "Param";
}
