#include "Function.hpp"

Function::Function(boost::shared_ptr<SymbolTable> st, vector< boost::shared_ptr<Param> > params, boost::shared_ptr<Type> returnType) : Callable(st, params) {
	_returnType = returnType;
	_params = params;
	_st = st;
}

boost::shared_ptr<Type> Function::getTypeName() {
	return _returnType;
}

string Function::getCallableFuncOrProc() {
	return "Function";
}
