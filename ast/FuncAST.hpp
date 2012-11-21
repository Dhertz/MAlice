#ifndef FUNC_AST
#define FUNC_AST

#include "ASTNode.hpp"
#include "CallParamsAST.hpp"
#include "../idents/Param.hpp"
#include "../idents/Function.hpp"
#include <vector>
#include <iostream>

// Forward declartion needed to avoid circular class declaration in ExprAST
class CallParamsAST;

class FuncAST : public ASTNode {
	boost::shared_ptr<SymbolTable> _st;
	string _name;
	boost::shared_ptr<CallParamsAST> _params;
public:
	FuncAST(boost::shared_ptr<SymbolTable> st, string name, boost::shared_ptr<CallParamsAST> params);
	void check();
	void parametersTypeCheck(boost::shared_ptr<Function> function);
};

#endif
