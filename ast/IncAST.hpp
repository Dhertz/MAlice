#ifndef INC_AST
#define INC_AST

#include "ASTNode.hpp"
#include "ExprAST.hpp"

class IncAST : public ASTNode {

public:
	IncAST(boost::shared_ptr<SymbolTable> st, boost::shared_ptr<ExprAST> expr);
};

#endif