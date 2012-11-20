#ifndef PRINT_AST
#define PRINT_AST

#include "ASTNode.hpp"
#include "ExprAST.hpp"
#include "CallParamsAST.hpp"
#include "../idents/Array.hpp"

class PrintAST : public ASTNode {
	SymbolTable* _st;
	string _name;
	CallParamsAST* _params;
	ExprAST* _element;
	ExprAST* _expr;
	Identifier* _type;
public:
	PrintAST(SymbolTable* st, string arrayName, ExprAST* element);
	PrintAST(SymbolTable* st, string funcName, CallParamsAST* params);
	PrintAST(SymbolTable* st, ExprAST* expr);
	void check();
	Identifier* getType();
};

#endif