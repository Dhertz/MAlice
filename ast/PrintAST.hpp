#ifndef PRINT_AST
#define PRINT_AST

#include "ASTNode.hpp"
#include "ExprAST.hpp"
#include "CallParamsAST.hpp"

class PrintAST : public ASTNode {

public:
	PrintAST(boost::shared_ptr<SymbolTable> st, string arrayName, ExprAST* element);
	PrintAST(boost::shared_ptr<SymbolTable> st, string funcName, CallParamsAST* params);
	PrintAST(boost::shared_ptr<SymbolTable> st, ExprAST* expr);
};

#endif