#ifndef PROC_H
#define PROC_H

#include "Callable.hpp"

class Proc : public Callable {
    vector< boost::shared_ptr<Param> > _params;
    boost::shared_ptr<SymbolTable> _st;
public:
    Proc(boost::shared_ptr<SymbolTable> st,
    	   vector< boost::shared_ptr<Param> > params);
    string getCallableFuncOrProc();
};

#endif
