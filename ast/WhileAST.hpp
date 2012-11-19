#ifndef WHILE_AST
#define WHILE_AST

#include "ASTNode.hpp"
#include "ExprAST.hpp"

class WhileAST : public ASTNode {

public:
	WhileAST(boost::shared_ptr<SymbolTable> st, ExprAST* expr);
};

#endif