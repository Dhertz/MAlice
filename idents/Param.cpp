#include "Param.hpp"

Param::Param(boost::shared_ptr<Type> type, string name) {
    _type = type;
    _name = name;
    _assLoc = "";
}

boost::shared_ptr<Type> Param::getType() {
    return _type;
}

string Param::getName() {
    return _name;
}

string Param::getBaseName() {
    return "Param";
}

string Param::getAssLoc() {
	return _assLoc;
}

void Param::setAssLoc(string assLoc) {
	_assLoc = assLoc;
}
