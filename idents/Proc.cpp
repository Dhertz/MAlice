#include "Proc.hpp"

Proc::Proc(vector<Param> params, SymbolTable* st) {
	_params = params;
	_st = st;
}

vector<Param> Proc::getParams() {
	return _params;
}

SymbolTable Proc::getTable() {
	return _st;
}

string Proc::getID() {
	return "Proc";
}