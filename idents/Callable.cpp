#include "Callable.hpp"

Callable::Callable(boost::shared_ptr<SymbolTable> st, vector< boost::shared_ptr<Param> > params) {
    _params = params;
    _st = st;
}

vector<  boost::shared_ptr<Param> > Callable::getParams() {
    return _params;
}

SymbolTable Callable::getTable() {
    return _st;
}

string Callable::getBaseName() {
    return "Callable";
}
