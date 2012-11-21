#ifndef TYPE_H
#define TYPE_H

#include "Identifier.hpp"

class Type : public Identifier {
public:
	string getBaseName();
	virtual string getTypeName() = 0;
};

#endif
