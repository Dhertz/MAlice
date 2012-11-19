#include "Param.hpp"

Param::Param(Type* type, string name) {
	_type = type;
	_name = name;
}

Type* Param::getType() {
	return _type;
}

string Param::getName() {
	return _name;
}

string Param::getID() {
	return "Param";
}
