#ifndef ASSEM_FUNC
#define ASSEM_FUNC

#include "AssemCom.hpp"
#include <list>
#include <vector>

/*
	AssemFunc
	---------
	Object that represents a function in ARM Assembler. It holds a list of it's
	commands, which can be manipulated using the add* methods. It keeps track of
	the registers free within it's own scope, and keeps track of how much the
	local stack pointer needs to be moved at the start of the function
*/
class AssemFunc {
	list<AssemCom> _comms;
	int _stackPointer;
	string _pointerByReg;
	vector<string> _freeRegs;
	string _name;
public:
	AssemFunc(string name);
	void initFreeRegs();
	void addFront(string name, vector<string> args);
	void addBack(string name, vector<string> args);
	void addListBack(list<AssemCom> l);
	void addListFront(list<AssemCom> l);
	void increaseStackPointer(int i);
	void increaseStackPointerByReg(string reg);
	int getStackPointer();
	string getName();
	void finalise();
	list<AssemCom>& getComms();
	void setFreeRegs(vector<string> v);
	void removeReg(string reg);
	vector<string> getFreeRegs();
	bool regIsFree(string reg);
	void freeReg(string reg);
};

#endif