#ifndef NUMBER_H
#define NUMBER_H

#include "Type.hpp"

class Number : public Type {
	int _min, _max;

public:
	Number(int min, int max);
	int getMin();
	int getMax();
	string getType();
};

#endif
