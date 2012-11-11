#ifndef VARIABLE_H
#define VARIABLE_H

#include "Identifier.hpp"
#include "Type.hpp"

class Variable : public Identifier {
	Type* _type;

public:
	Variable(Type* type);
	Type* getType();
	string getID();
};

#endif
