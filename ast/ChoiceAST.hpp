#ifndef CHOICE_AST
#define CHOICE_AST

#include "ASTNode.hpp"
#include "ExprAST.hpp"

class ChoiceAST : public ASTNode {

public:
	ChoiceAST(SymbolTable* st, ExprAST* expr);
};

#endif