#ifndef IF_AST
#define IF_AST

class IfAST : public ASTNode {

public:
	IfAST(SymbolTable* st, ExprAST* expr);
};

#endif