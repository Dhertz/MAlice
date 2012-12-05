#ifndef ASSEM_FUNC
#define ASSEM_FUNC

#include "AssemCom.hpp"
#include <list>

class AssemFunc {
list<AssemCom> _comms;
int stackPointer;
public:
	void addFront(string name, vector<string> args);
	void addBack(string name, vector<string> args);
	void setStackPointer(int i);
	int getStackPointer();
};

#endif