#ifndef FUNCTION_H
#define FUNCTION_H

#include "Callable.hpp"

class Function : public Callable {
	boost::shared_ptr<Type> _returnType;
	vector< boost::shared_ptr<Param> > _params;
	boost::shared_ptr<SymbolTable> _st;

public:
	Function(boost::shared_ptr<SymbolTable> st, vector< boost::shared_ptr<Param> > params, boost::shared_ptr<Type> returnType);
	boost::shared_ptr<Type> getType();
	string getCallableFuncOrProc();
};

#endif
