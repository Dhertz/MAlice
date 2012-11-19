#ifndef PARAM_H
#define PARAM_H

#include "Type.hpp"

class Param : public Identifier {
	Type* _type;
	string _name;
public:
	Param(Type* type, string name);
	Type* getType();
	string getName();
	string getID();
};

#endif
