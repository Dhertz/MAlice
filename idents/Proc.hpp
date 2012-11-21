#ifndef Proc_H
#define Proc_H

#include "Identifier.hpp"
#include "Type.hpp"
#include "Param.hpp"
#include "SymbolTable.hpp"
#include <vector>

using namespace std;

class Proc : public Identifier {
	vector< boost::shared_ptr<Param> > _params;
	boost::shared_ptr<SymbolTable> _st;

public:
	Proc( boost::shared_ptr<SymbolTable> st, vector< boost::shared_ptr<Param> > params);
	string getID();
	vector< boost::shared_ptr<Param> > getParams();
	SymbolTable getTable();
};

#endif