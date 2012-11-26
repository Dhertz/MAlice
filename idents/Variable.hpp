#ifndef VARIABLE_H
#define VARIABLE_H

#include "Type.hpp"

class Variable : public Identifier {
    boost::shared_ptr<Type> _type;

public:
    Variable(boost::shared_ptr<Type> type);
    boost::shared_ptr<Type> getTypeName();
    string getBaseName();
};

#endif
