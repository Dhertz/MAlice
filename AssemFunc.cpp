#include "AssemFunc.hpp"
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <algorithm>

AssemFunc::AssemFunc(string name) {
	_name = name;
	_stackPointer = 0;
	_pointerByReg = "";
	initFreeRegs();
}

void AssemFunc::initFreeRegs() {
	string regs[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9",
					 "r10"};

	_freeRegs = vector<string>(regs, regs + sizeof(regs) / sizeof(string));
}

void AssemFunc::addFront(string name, vector<string> args) {
	_comms.push_front(AssemCom(name, args));
}

void AssemFunc::addBack(string name, vector<string> args) {
	_comms.push_back(AssemCom(name, args));
}

void AssemFunc::addListBack(list<AssemCom> l) {
	_comms.splice(_comms.end(), l);
}

void AssemFunc::addListFront(list<AssemCom> l) {
	_comms.splice(_comms.begin(), l);
}

// Increase the running count of the stack depth. This is called from ASTVisitor
// whenever something is put onto the stack
void AssemFunc::increaseStackPointer(int i) {
	_stackPointer += i;
}

// The stack pointer might be moved an amount that is unknown at compile time, 
// we allow for that here
void AssemFunc::increaseStackPointerByReg(string reg) {
	_pointerByReg = reg;
}

int AssemFunc::getStackPointer() {
	return _stackPointer;
}

string AssemFunc::getName() {
	return _name;
}

void AssemFunc::finalise() {
	// Add the necessary commands to the top and bottom of the function, 
	// depending on if the function is the main entry point, and whether the
	// local stack pointer needs to be moved or not

	// NB: Front intrsuctions are done in reverse

	if (_name == "main") {
		vector<string> args;
		args.push_back("r0");
		args.push_back("#0");
		addBack("mov", args);
	}

	if (_stackPointer > 0) {
		// Only do this if we actually need to move the pointer, avoid 
		// sub sp sp #0 and the like.
		vector<string> spArgs(2, "sp");
		spArgs.push_back("#" + boost::lexical_cast<string>(_stackPointer));
		addFront("sub", spArgs);

		vector<string> fpArgs;
		fpArgs.push_back("fp");
		fpArgs.push_back("sp");
		fpArgs.push_back("#4");
		addFront("add", fpArgs);

		fpArgs.clear();
		fpArgs.push_back("sp");
		fpArgs.push_back("fp");
		fpArgs.push_back("#4");
		addBack("sub", fpArgs);
	} else if (_pointerByReg != "") {

		if (_pointerByReg[0] == '.') {
			// size is in a label
			string tempReg = _freeRegs.front();

			vector<string> spArgs(2, "sp");
			spArgs.push_back(tempReg);
			addFront("sub", spArgs);

			spArgs.clear();
			spArgs.push_back(tempReg);
			spArgs.push_back(_pointerByReg);
			addFront("ldr", spArgs);
		} else {
			vector<string> spArgs(2, "sp");
			spArgs.push_back(_pointerByReg);
			addFront("sub", spArgs);
		}

		vector<string> fpArgs;
		fpArgs.push_back("fp");
		fpArgs.push_back("sp");
		fpArgs.push_back("#4");
		addFront("add", fpArgs);

		fpArgs.clear();
		fpArgs.push_back("sp");
		fpArgs.push_back("fp");
		fpArgs.push_back("#4");
		addBack("sub", fpArgs);
	}

	if (_name == "main") {
		vector<string> args;
		args.push_back("sp!");
		args.push_back("{fp, lr}");
		addFront("stmfd", args);

		args.clear();
		args.push_back("sp!");
		args.push_back("{fp, pc}");
		addBack("ldmfd", args);

		addFront("main:", vector<string>());

		vector<string> globArgs(1, _name);
		addFront(".global", globArgs);
	} else {
		vector<string> args;
		args.push_back("sp!");
		args.push_back("{r4-r10, fp, lr}");
		addFront("stmfd", args);

		args.clear();
		args.push_back("sp!");
		args.push_back("{r4-r10, fp, pc}");
		addBack("ldmfd", args);

		addFront(_name + ":", vector<string>());
	}

	vector<string> alignArgs(1, "2");
	addFront(".align", alignArgs);

}

list<AssemCom>& AssemFunc::getComms() {
	return _comms;
}

void AssemFunc::setFreeRegs(vector<string> v) {
	_freeRegs = v;
}

void AssemFunc::removeReg(string reg) {
	_freeRegs.erase(
		std::remove(_freeRegs.begin(), _freeRegs.end(), reg), _freeRegs.end());
}

vector<string> AssemFunc::getFreeRegs() {
	return _freeRegs;
}

bool AssemFunc::regIsFree(string reg) {
	return find(_freeRegs.begin(), _freeRegs.end(), reg) != _freeRegs.end();
}

void AssemFunc::freeReg(string reg) {
	_freeRegs.push_back(reg);
}
