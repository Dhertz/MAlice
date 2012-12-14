#include "AssemCom.hpp"
#include <iostream>

AssemCom::AssemCom(string name, vector<string> args) {
	_name = name;
	_arity = args.size();
	_args = args;
}

string AssemCom::getName() {
	return _name;
}

int AssemCom::getArity() {
	return _arity;
}

vector<string>& AssemCom::getArgs() {
	return _args;
}

// Allow equality check on commands for use in various places across code
// generation
bool operator== (AssemCom& com1, AssemCom& com2) {
	return (com1.getName() == com2.getName() && 
			com1.getArgs() == com2.getArgs());
}
