#ifndef STDIN_AST
#define STDIN_AST

#include "ASTNode.hpp"
#include "ExprAST.hpp"

class StdinAST : public ASTNode {

public:
	StdinAST(boost::shared_ptr<SymbolTable> st, ExprAST* expr);
};

#endif