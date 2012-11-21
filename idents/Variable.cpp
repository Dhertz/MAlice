#include "Variable.hpp"

Variable::Variable(boost::shared_ptr<Type> type) {
	_type = type;
}

boost::shared_ptr<Type> Variable::getType() {
	return _type;
}

string Variable::getID() {
	return "Variable";
}
