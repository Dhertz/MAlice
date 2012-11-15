#ifndef HEADER_PARAMS_AST
#define HEADER_PARAMS_AST

class HeaderParamsAST : public ASTNode {

public:
	HeaderParamsAST(SymbolTable* st, pANTLR3_BASE_TREE tree);
};

#endif