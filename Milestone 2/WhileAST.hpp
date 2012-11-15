#ifndef WHILE_AST
#define WHILE_AST

class WhileAST : public ASTNode {

public:
	WhileAST(SymbolTable* st, ExprAST* expr);
};

#endif