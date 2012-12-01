#include "AssemCom.hpp"

AssemCom::AssemCom(string name, int arity, vector<string> args) {
	_name = name;
	_arity = arity;
	_args = args;
}

string AssemCom::getName() {
	return _name;
}

int AssemCom::getArity() {
	return _arity;
}

vector<string> AssemCom::getArgs() {
	return _args;
}
