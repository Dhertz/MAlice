#ifndef ARRAY_H
#define ARRAY_H

#include "Type.hpp"

class Array : public Type {
	Type* _elemType;
	int _elems;

public:
	Array(Type* elemType, int elems);
	Type* getElemType();
	int getSize();
	string getType();
};

#endif
