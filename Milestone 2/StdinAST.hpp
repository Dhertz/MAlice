#ifndef STDIN_AST
#define STDIN_AST

class StdinAST : public ASTNode {

public:
	StdinAST(SymbolTable* st, ExprAST* expr);
};

#endif