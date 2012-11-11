#ifndef PARAM_H
#define PARAM_H

#include "Type.hpp"

class Param : public Identifier {
	Type* _type;
public:
	Param(Type* type);
	Type* getType();
	string getID();
};

#endif
