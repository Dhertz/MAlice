#ifndef ASSEM_FUNC
#define ASSEM_FUNC

#include "AssemCom.hpp"
#include <list>
#include <vector>

class AssemFunc {
list<AssemCom> _comms;
int stackPointer;
vector<string> usedRegs;
public:
	AssemFunc();
	void addFront(string name, vector<string> args);
	void addBack(string name, vector<string> args);
	void addListBack(list<AssemCom> l);
	void increaseStackPointer(int i);
	int  getStackPointer();
	void finalise();
	list<AssemCom> getComms();
};

#endif