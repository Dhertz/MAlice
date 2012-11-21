#include "Variable.hpp"

Variable::Variable(boost::shared_ptr<Type> type) {
	_type = type;
}

boost::shared_ptr<Type> Variable::getTypeName() {
	return _type;
}

string Variable::getBaseName() {
	return "Variable";
}
