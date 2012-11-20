#ifndef ARRAY_ASSIGN_AST
#define ARRAY_ASSIGN_AST

#include "ASTNode.hpp"
#include "ExprAST.hpp"
#include "../idents/Array.hpp"

class ArrayAssignAST : public ASTNode {
	SymbolTable* _st;
	string _name;
	ExprAST* _element;
	ExprAST* _value;
	Array* _arrObj;
public:
	ArrayAssignAST(SymbolTable* st, string name, ExprAST* element, ExprAST* value);
	void check();
};

#endif