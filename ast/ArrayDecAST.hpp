#ifndef ARRAY_DEC_AST
#define ARRAY_DEC_AST

#include "ASTNode.hpp"
#include "ExprAST.hpp"
#include "../idents/Array.hpp"

class ArrayDecAST : public ASTNode {
	SymbolTable* _st;
	ExprAST* _length;
	string _name;
	string _elemType;
	Array* _arrObj;
public:
	ArrayDecAST(SymbolTable* st, ExprAST* expr, string name, string typeName);
	void check();
};

#endif