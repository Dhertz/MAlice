#include "Scalar.hpp"

Scalar::Scalar(int min, int max){
	_min = min;
	_max = max;
}

int Scalar::getMin() {
	return _min;
}

int Scalar::getMax() {
	return _max;
}
