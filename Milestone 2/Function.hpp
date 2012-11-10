#ifndef FUNCTION_H
#define FUNCTION_H

#include "Identifier.hpp"
#include "Type.hpp"
#include "Param.hpp"
#include "SymbolTable.hpp"

class Function : public Identifier {
	Type returnType;
	Param params[];
	SymbolTable st;
};

#endif
