#ifndef ASSEM_FUNC
#define ASSEM_FUNC

#include "AssemCom.hpp"
#include <list>
#include <vector>

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