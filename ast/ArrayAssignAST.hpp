#ifndef ARRAY_ASSIGN_AST
#define ARRAY_ASSIGN_AST

#include "ASTNode.hpp"
#include "ExprAST.hpp"

class ArrayAssignAST : public ASTNode {

public:
	ArrayAssignAST(SymbolTable* st, string name, ExprAST* element, ExprAST* value);
};

#endif