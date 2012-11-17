#ifndef FUNC_DEC_AST
#define FUNC_DEC_AST

#include "HeaderParamsAST.hpp"

class FuncDecAST : public ASTNode {

public:
	FuncDecAST(SymbolTable* st, string name, HeaderParamsAST* params, string returnType);
};

#endif