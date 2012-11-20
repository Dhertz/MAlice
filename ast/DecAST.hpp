#ifndef DEC_AST
#define DEC_AST

#include "ASTNode.hpp"
#include "ExprAST.hpp"

class DecAST : public ASTNode {
	SymbolTable* _st;
	ExprAST* _expr;
public:
	DecAST(SymbolTable* st, ExprAST* expr);
	void check();
};

#endif