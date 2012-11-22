#ifndef CALLABLE_H
#define CALLABLE_H

#include "Identifier.hpp"
#include "Param.hpp"
#include "../SymbolTable.hpp"
#include <vector>

class Callable : public Identifier {
    vector< boost::shared_ptr<Param> > _params;
    boost::shared_ptr<SymbolTable> _st;
public:
    Callable(boost::shared_ptr<SymbolTable> st, vector< boost::shared_ptr<Param> > params);
    string getBaseName();
    vector< boost::shared_ptr<Param> > getParams();
    SymbolTable getTable();
	virtual string getCallableFuncOrProc();
};

#endif