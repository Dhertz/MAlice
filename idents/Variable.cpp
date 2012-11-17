#include "Variable.hpp"

Variable::Variable(Type* type) {
	_type = type;
}

Type* Variable::getType() {
	return _type;
}

string Variable::getID() {
	return "Variable";
}
