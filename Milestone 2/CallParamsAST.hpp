#ifndef CALL_PARARMS_AST
#define CALL_PARARMS_AST

class CallParamsAST : public ASTNode {

public:
	CallParamsAST(SymbolTable* st, pANTLR3_BASE_TREE tree);
};

#endif