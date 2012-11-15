#ifndef INC_AST
#define INC_AST

class IncAST : public ASTNode {

public:
	IncAST(SymbolTable* st, ExprAST* expr);
};

#endif