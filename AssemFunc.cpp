#include "AssemFunc.hpp"

#include <vector>

void AssemFunc::addFront(string name, vector<string> args) {
	_comms.push_front(AssemCom(name, args.size(), args));
}

void AssemFunc::addBack(string name, vector<string> args) {
	_comms.push_back(AssemCom(name, args.size(), args));
}

void AssemFunc::setStackPointer(int i) {
	stackPointer = i;
}

int AssemFunc::getStackPointer() {
	return stackPointer;
}
