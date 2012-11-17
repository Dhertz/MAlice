#ifndef ARRAY_DEC_AST
#define ARRAY_DEC_AST

#include "ASTNode.hpp"
#include "ExprAST.hpp"

class ArrayDecAST : public ASTNode {

public:
	ArrayDecAST(SymbolTable* st, ExprAST* expr, string typeName);
};

#endif