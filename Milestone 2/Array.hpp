#ifndef ARRAY_H
#define ARRAY_H

#include "Type.hpp"

class Array : Type {
	Type _elemType;
	int _elems;

public:
	Array(Type elemType, int elems);
	int getSize();
	Type getType();
};

#endif
