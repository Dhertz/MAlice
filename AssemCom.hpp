#ifndef ASSEM_COM_H
#define ASSEM_COM_H 

#include <string>
#include <vector>

using namespace std;

/*
	AssemCom
	--------
	Object representing a command in ARM Assembler. Holds the command name, it's
	arity and it's arguments.
*/
class AssemCom {
	string _name;
	int _arity;
	vector<string> _args;
public:
	AssemCom(string name, vector<string> args);
	string getName();
	int getArity();
	vector<string>& getArgs();
	friend bool operator== (AssemCom& com1, AssemCom& com2);
};

#endif
