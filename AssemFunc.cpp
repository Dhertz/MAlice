#include "AssemFunc.hpp"
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <algorithm>

AssemFunc::AssemFunc(string name) {
	_name = name;
	stackPointer = 0;
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

void AssemFunc::increaseStackPointer(int i) {
	stackPointer += i;
}

int AssemFunc::getStackPointer() {
	return stackPointer;
}

string AssemFunc::getName() {
	return _name;
}

void AssemFunc::finalise() {
	// Front intrsuctions are done in reverse

	if (_name == "main") {
		vector<string> args;
		args.push_back("r0");
		args.push_back("#0");
		addBack("mov", args);
	}

	if (stackPointer > 0) {
		vector<string> spArgs(2, "sp");
		spArgs.push_back("#" + boost::lexical_cast<string>(stackPointer));
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
