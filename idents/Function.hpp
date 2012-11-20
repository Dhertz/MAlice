#ifndef FUNCTION_H
#define FUNCTION_H

#include "Identifier.hpp"
#include "Type.hpp"
#include "Param.hpp"
#include "SymbolTable.hpp"
#include <vector>

using namespace std;

class Function : public Identifier {
	boost::shared_ptr<Type> _returnType;
	vector<Param> _params;
	boost::shared_ptr<SymbolTable> _st;

public:
	Function(boost::shared_ptr<Type> returnType, vector<Param> params, boost::shared_ptr<SymbolTable> st);
	boost::shared_ptr<Type> getType();
	vector<Param> getParams();
	SymbolTable getTable();
};

#endif
