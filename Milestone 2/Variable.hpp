#ifndef VARIABLE_H
#define VARIABLE_H

#include "Identifier.hpp"
#include "Type.hpp"
#include "NullType.hpp"

class Variable : public Identifier {
	Type _type;

public:
	Variable();
	Variable(Type type);
	Type getType();
	void setType(Type type);
};

#endif
