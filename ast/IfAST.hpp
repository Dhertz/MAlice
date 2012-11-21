#ifndef IF_AST
#define IF_AST

#include "ASTNode.hpp"
#include "ExprAST.hpp"

class IfAST : public ASTNode {
	boost::shared_ptr<SymbolTable> _st;
	boost::shared_ptr<ExprAST> _expr;
public:
	IfAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr);
	void check();
};

#endif