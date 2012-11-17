#ifndef RETURN_AST
#define RETURN_AST

#include "ASTNode.hpp"
#include "ExprAST.hpp"

class ReturnAST : public ASTNode {

public:
	ReturnAST(SymbolTable* st, ExprAST* expr);
};

#endif