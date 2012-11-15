#ifndef DEC_AST
#define DEC_AST

class DecAST : public ASTNode {

public:
	DecAST(SymbolTable* st, ExprAST* expr);
};

#endif