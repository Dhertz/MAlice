#ifndef FUNC_AST
#define FUNC_AST

#include "ASTNode.hpp"
#include "CallParamsAST.hpp"

class FuncAST : public ASTNode {

public:
	FuncAST(boost::shared_ptr<SymbolTable> st, string name, boost::shared_ptr<CallParamsAST> params);
};

#endif
