#ifndef IF_AST
#define IF_AST

#include "ASTNode.hpp"
#include "ExprAST.hpp"

class IfAST : public ASTNode {

public:
	IfAST(boost::shared_ptr<SymbolTable> st, ExprAST* expr);
};

#endif