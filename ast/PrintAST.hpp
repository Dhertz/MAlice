#ifndef PRINT_AST
#define PRINT_AST

#include "ASTNode.hpp"
#include "ExprAST.hpp"
#include "CallParamsAST.hpp"

class PrintAST : public ASTNode {

public:
	PrintAST(boost::shared_ptr<SymbolTable> st, string arrayName, boost::shared_ptr<ExprAST> element);
	PrintAST(boost::shared_ptr<SymbolTable> st, string funcName, boost::shared_ptr<CallParamsAST> params);
	PrintAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr);
};

#endif