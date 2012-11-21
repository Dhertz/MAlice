#ifndef DEC_AST
#define DEC_AST

#include "ASTNode.hpp"
#include "ExprAST.hpp"

class DecAST : public ASTNode {
	boost::shared_ptr<SymbolTable> _st;
	boost::shared_ptr<ExprAST> _expr;
public:
	DecAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr);
	void check();
};

#endif