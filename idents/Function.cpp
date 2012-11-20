#include "Function.hpp"

Function::Function(Type* returnType, vector<Param> params, SymbolTable* st) {
	_returnType = returnType;
	_params = params;
	_st = st;
}

Type* Function::getType() {
	return _returnType;
}

vector<Param> Function::getParams() {
	return _params;
}

SymbolTable Function::getTable() {
	return _st;
}

string Function::getID() {
	return "Function";
}