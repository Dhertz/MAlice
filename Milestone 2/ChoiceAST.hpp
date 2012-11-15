#ifndef CHOICE_AST
#define CHOICE_AST

class ChoiceAST : public ASTNode {

public:
	ChoiceAST(SymbolTable* st, ExprAST* expr);
};

#endif