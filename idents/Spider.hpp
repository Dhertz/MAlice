#ifndef SPIDER_H
#define SPIDER_H

#include "Array.hpp"
#include "Variable.hpp"

class Spider : public Variable {
	boost::shared_ptr<Array> _array;
	boost::shared_ptr<Type> _elemType;

public:
	Spider(boost::shared_ptr<Type> elemType);
	boost::shared_ptr<Array> getArray();
	boost::shared_ptr<Type> getElemType();
	string getTypeName();
	string getBaseName();
};

#endif