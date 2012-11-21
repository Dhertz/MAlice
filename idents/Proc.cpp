#include "Proc.hpp"

Proc::Proc(boost::shared_ptr<SymbolTable> st, vector< boost::shared_ptr<Param> > params) {
	_params = params;
	_st = st;
}

vector<  boost::shared_ptr<Param> > Proc::getParams() {
	return _params;
}

SymbolTable Proc::getTable() {
	return _st;
}

string Proc::getID() {
	return "Proc";
}