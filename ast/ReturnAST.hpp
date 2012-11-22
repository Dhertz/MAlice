#ifndef RETURN_AST
#define RETURN_AST

#include "ExprAST.hpp"

class ReturnAST : public ASTNode {
	boost::shared_ptr<SymbolTable> _st;
	boost::shared_ptr<ExprAST> _expr;
    int _lineNo;
	void check();
	void checkFunctionType(boost::shared_ptr<ASTNode> parent);
public:
	ReturnAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr, boost::shared_ptr<ASTNode> parent, int lineNo);
};

#endif
