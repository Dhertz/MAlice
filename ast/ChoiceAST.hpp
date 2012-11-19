#ifndef CHOICE_AST
#define CHOICE_AST

#include "ASTNode.hpp"
#include "ExprAST.hpp"

class ChoiceAST : public ASTNode {

public:
	ChoiceAST(boost::shared_ptr<SymbolTable> st, ExprAST* expr);
};

#endif