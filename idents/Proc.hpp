#ifndef Proc_H
#define Proc_H

#include "Callable.hpp"

using namespace std;

class Proc : public Callable {
	vector< boost::shared_ptr<Param> > _params;
	boost::shared_ptr<SymbolTable> _st;

public:
	Proc(boost::shared_ptr<SymbolTable> st, vector< boost::shared_ptr<Param> > params);
};

#endif