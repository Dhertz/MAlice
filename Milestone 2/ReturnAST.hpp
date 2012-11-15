#ifndef RETURN_AST
#define RETURN_AST

class ReturnAST : public ASTNode {

public:
	ReturnAST(SymbolTable* st, ExprAST* expr);
};

#endif