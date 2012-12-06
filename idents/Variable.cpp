#include "Variable.hpp"

Variable::Variable(boost::shared_ptr<Type> type) {
    _type = type;
	_assLoc = "";
}

boost::shared_ptr<Type> Variable::getTypeName() {
    return _type;
}

string Variable::getBaseName() {
    return "Variable";
}

string Variable::getAssLoc() {
	return _assLoc;
}

void Variable::setAssLoc(string assLoc) {
	_assLoc = assLoc;
}

int Variable::getVal() {
	return _val;
}

void Variable::setVal(int val) {
	_val = val;
}
