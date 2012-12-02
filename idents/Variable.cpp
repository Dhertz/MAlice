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

string getAssLoc() {
	return _assLoc;
}

void setAssLoc(string assLoc) {
	_assLoc = assLoc;
}
