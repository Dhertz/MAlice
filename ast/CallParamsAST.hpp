#ifndef CALL_PARARMS_AST
#define CALL_PARARMS_AST

#include "ASTNode.hpp"
#include <antlr3.h>

class CallParamsAST : public ASTNode {

public:
	CallParamsAST(SymbolTable* st, pANTLR3_BASE_TREE tree);
};

#endif