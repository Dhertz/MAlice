#ifndef FUNC_AST
#define FUNC_AST

#include "ASTNode.hpp"
#include "CallParamsAST.hpp"
#include "../idents/Param.hpp"
#include "../idents/Function.hpp"
#include <vector>
#include <iostream>

class FuncAST : public ASTNode {
	SymbolTable* _st;
	string _name;
	CallParamsAST* _params;
public:
	FuncAST(SymbolTable* st, string name, CallParamsAST* params);
	void check();
	bool parametersTypeCheck(Function* function);
};

#endif