#ifndef CALLABLE_H
#define CALLABLE_H

#include <vector>
#include "Param.hpp"
#include "../SymbolTable.hpp"

class Callable : public Identifier {
    vector< boost::shared_ptr<Param> > _params;
    boost::shared_ptr<SymbolTable> _st;
public:
    Callable(boost::shared_ptr<SymbolTable> st, vector< boost::shared_ptr<Param> > params);
    vector< boost::shared_ptr<Param> > getParams();
    SymbolTable getTable();
    string getBaseName();
	virtual string getCallableFuncOrProc() = 0;
};

#endif
