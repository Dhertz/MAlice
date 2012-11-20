#ifndef RETURN_AST
#define RETURN_AST

#include "ASTNode.hpp"
#include "ExprAST.hpp"

class ReturnAST : public ASTNode {

public:
	ReturnAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr);
};

#endif