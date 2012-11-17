#ifndef HEADER_PARAMS_AST
#define HEADER_PARAMS_AST

#include "ASTNode.hpp"
#include <antlr3.h>

class HeaderParamsAST : public ASTNode {

public:
	HeaderParamsAST(SymbolTable* st, pANTLR3_BASE_TREE tree);
};

#endif