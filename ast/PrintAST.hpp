#ifndef PRINT_AST
#define PRINT_AST

#include "ASTNode.hpp"
#include "ExprAST.hpp"
#include "CallParamsAST.hpp"
#include "../idents/Array.hpp"

class PrintAST : public ASTNode {
	boost::shared_ptr<SymbolTable> _st;
	string _name;
	boost::shared_ptr<CallParamsAST> _params;
	boost::shared_ptr<ExprAST> _element;
	boost::shared_ptr<ExprAST> _expr;
	boost::shared_ptr<Identifier> _type;
public:
	PrintAST(boost::shared_ptr<SymbolTable> st, string arrayName, boost::shared_ptr<ExprAST> element);
	PrintAST(boost::shared_ptr<SymbolTable> st, string funcName, boost::shared_ptr<CallParamsAST> params);
	PrintAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr);
	void check();
	boost::shared_ptr<Identifier> getType();
};

#endif