#include "AssemFunc.hpp"
#include <iostream>
#include <boost/lexical_cast.hpp>

AssemFunc::AssemFunc(string name) {
	_name = name;
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
	// Front intrsuctions are done in reverse

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

	vector<string> args;
	args.push_back("sp!");
	args.push_back("{fp, lr}");
	addFront("stmfd", args);

	args.clear();
	args.push_back("sp!");
	args.push_back("{fp, pc}");
	addBack("ldmfd", args);

	addFront(_name + ":", vector<string>());

	if (_name == "main") {
		vector<string> globArgs(1, _name);
		addFront(".global", globArgs);
	}

	vector<string> alignArgs(1, "2");
	addFront(".align", alignArgs);

}

list<AssemCom> AssemFunc::getComms() {
	return _comms;
}
