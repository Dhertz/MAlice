#ifndef CHOICE_AST
#define CHOICE_AST

#include "ASTNode.hpp"
#include "ExprAST.hpp"

class ChoiceAST : public ASTNode {

public:
	ChoiceAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr);
};

#endif