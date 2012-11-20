#include "Function.hpp"

Function::Function(boost::shared_ptr<Type> returnType, vector<Param> params, boost::shared_ptr<SymbolTable> st) {
	_returnType = returnType;
	_params = params;
	_st = st;
}

boost::shared_ptr<Type> Function::getType() {
	return _returnType;
}

vector<Param> Function::getParams() {
	return _params;
}

SymbolTable Function::getTable() {
	return _st;
}
