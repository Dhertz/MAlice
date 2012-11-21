#ifndef WHILE_AST
#define WHILE_AST

#include "ASTNode.hpp"
#include "ExprAST.hpp"

class WhileAST : public ASTNode {
	boost::shared_ptr<SymbolTable> _st;
	boost::shared_ptr<ExprAST> _expr;
public:
	WhileAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr);
	void check();
};

#endif