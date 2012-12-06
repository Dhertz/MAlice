#ifndef ASSEM_COM_H
#define ASSEM_COM_H 

#include <string>
#include <vector>

using namespace std;

class AssemCom {
	string _name;
	int _arity;
	vector<string> _args;
public:
	AssemCom(string name, vector<string> args);
	string getName();
	int getArity();
	vector<string> getArgs();
};

#endif
