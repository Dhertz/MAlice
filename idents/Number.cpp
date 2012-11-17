#include "Number.hpp"

Number::Number(int min, int max){
	_min = min;
	_max = max;
}

int Number::getMin() {
	return _min;
}

int Number::getMax() {
	return _max;
}

string Number::getType() {
	return "Number";
}
