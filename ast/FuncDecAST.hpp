#ifndef FUNC_DEC_AST
#define FUNC_DEC_AST

#include "ASTNode.hpp"
#include "HeaderParamsAST.hpp"
#include "../idents/Function.hpp"
#include <vector>

class FuncDecAST : public ASTNode {

public:
	SymbolTable* _st;
	string _name;
	string _returnType;
	HeaderParamsAST* _params;
	Function* _funcObj;

	FuncDecAST(SymbolTable* st, string name, HeaderParamsAST* params, string returnType);
	void check();
};

#endif