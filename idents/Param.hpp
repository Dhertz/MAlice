#ifndef PARAM_H
#define PARAM_H

#include "Type.hpp"

class Param : public Identifier {
    boost::shared_ptr<Type> _type;
    string _name;
public:
    Param(boost::shared_ptr<Type> type, string name);
    boost::shared_ptr<Type> getType();
    string getName();
    string getBaseName();
};

#endif
