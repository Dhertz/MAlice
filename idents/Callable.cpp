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

// TODO: looks like these both have a similar purpose, are they both needed? ~Charlie

string Callable::getBaseName() {
    return "Callable";
}
