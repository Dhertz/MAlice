#ifndef FUNC_AST
#define FUNC_AST

#include "ASTNode.hpp"
#include "CallParamsAST.hpp"

class FuncAST : public ASTNode {

public:
	FuncAST(boost::shared_ptr<SymbolTable> st, string name, CallParamsAST* params);
};

#endif