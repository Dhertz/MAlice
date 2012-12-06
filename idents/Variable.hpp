#ifndef VARIABLE_H
#define VARIABLE_H

#include "Type.hpp"

class Variable : public Identifier {
    boost::shared_ptr<Type> _type;
	string _assLoc;
	int _val;
public:
    Variable(boost::shared_ptr<Type> type);
    boost::shared_ptr<Type> getTypeName();
    string getBaseName();
	string getAssLoc();
	void setAssLoc(string assLoc);
	int getVal();
	void setVal(int val);
};

#endif
