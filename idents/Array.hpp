#ifndef ARRAY_H
#define ARRAY_H

#include "Type.hpp"

class Array : public Type {
	Type* _elemType;

public:
	Array(Type* elemType);
	Type* getElemType();
	string getID();
};

#endif
