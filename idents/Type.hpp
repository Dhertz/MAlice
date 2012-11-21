#ifndef TYPE_H
#define TYPE_H

#include "Identifier.hpp"

class Type : public Identifier {
public:
	string getID();
	virtual string getType() = 0;
};

#endif
