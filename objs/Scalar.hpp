#ifndef SCALAR_H
#define SCALAR_H

#include "Type.hpp"

class Scalar : public Type {
	int _min, _max;

public:
	Scalar(int min, int max);
	int getMin();
	int getMax();
	string getType();
};

#endif
