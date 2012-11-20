#ifndef RETURN_AST
#define RETURN_AST

#include "ASTNode.hpp"
#include "ExprAST.hpp"

class ReturnAST : public ASTNode {
	SymbolTable _st;
	ExprAST* _expr;
public:
	ReturnAST(SymbolTable* st, ExprAST* expr);
};

#endif