#ifndef ARRAY_DEC_AST
#define ARRAY_DEC_AST

#include "ASTNode.hpp"
#include "ExprAST.hpp"

class ArrayDecAST : public ASTNode {

public:
	ArrayDecAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr, string typeName);
};

#endif