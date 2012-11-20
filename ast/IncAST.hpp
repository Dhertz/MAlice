#ifndef INC_AST
#define INC_AST

#include "ASTNode.hpp"
#include "ExprAST.hpp"

class IncAST : public ASTNode {
	SymbolTable* _st;
	ExprAST* _expr;
public:
	IncAST(SymbolTable* st, ExprAST* expr);
	void check();
};

#endif