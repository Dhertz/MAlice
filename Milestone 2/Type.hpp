#ifndef TYPE_H
#define TYPE_H

#include "Identifier.hpp"
#include "string"

using namespace std;

class Type : public Identifier {
public:
	virtual string getID();
};

#endif
