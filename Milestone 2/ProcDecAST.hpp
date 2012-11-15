#ifndef PROC_DEC_AST
#define PROC_DEC_AST

#include "HeaderParamsAST.hpp"

class ProcDecAST : public ASTNode {

public:
	ProcDecAST(SymbolTable* st, string name, HeaderParamsAST* params);
};

#endif