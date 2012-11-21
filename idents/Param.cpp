#include "Param.hpp"

Param::Param(boost::shared_ptr<Type> type, string name) {
	_type = type;
	_name = name;
}

boost::shared_ptr<Type> Param::getTypeName() {
	return _type;
}

string Param::getName() {
	return _name;
}

string Param::getBaseName() {
	return "Param";
}
