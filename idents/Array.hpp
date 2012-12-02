#ifndef ARRAY_H
#define ARRAY_H

#include "Type.hpp"

class Array : public Type {
    boost::shared_ptr<Type> _elemType;
	string _assLoc;
public:
    Array(boost::shared_ptr<Type> elemType);
    boost::shared_ptr<Type> getElemType();
    string getTypeName();
	string getAssLoc();
	void setAssLoc(string assLoc);
};

#endif
