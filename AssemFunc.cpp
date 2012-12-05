#include "AssemFunc.hpp"
#include <iostream>

AssemFunc::AssemFunc() {
	stackPointer = 0;
}

void AssemFunc::addFront(string name, vector<string> args) {
	_comms.push_front(AssemCom(name, args.size(), args));
}

void AssemFunc::addBack(string name, vector<string> args) {
	_comms.push_back(AssemCom(name, args.size(), args));
}

void AssemFunc::addListBack(list<AssemCom> l) {
	_comms.splice(_comms.end(), l);
}

void AssemFunc::increaseStackPointer(int i) {
	stackPointer += i;
}

int AssemFunc::getStackPointer() {
	return stackPointer;
}

void AssemFunc::finalise() {
	cout << "Function pre-amble" << endl;
	// TODO
	cout << "Function post-amble" << endl;
}

list<AssemCom> AssemFunc::getComms() {
	return _comms;
}
