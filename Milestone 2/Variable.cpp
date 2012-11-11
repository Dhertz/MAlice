#include "Variable.hpp"

Variable::Variable() {
	NullType null;
	_type = null;
}

Variable::Variable(Type type){
	_type = type;
}

Type Variable::getType() {
	return _type;
}

void Variable::setType(Type type) {
	_type = type;
}

string getID() {
	return "Variable";
}