#include "Proc.hpp"

Proc::Proc(boost::shared_ptr<SymbolTable> st, vector< boost::shared_ptr<Param> > params) : Callable(st, params) {
	_params = params;
	_st = st;
}
