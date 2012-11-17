#ifndef FUNC_AST
#define FUNC_AST

#include "ASTNode.hpp"
#include "CallParamsAST.hpp"

class FuncAST : public ASTNode {

public:
	FuncAST(SymbolTable* st, string name, CallParamsAST* params);
};

#endif