#ifndef DEC_AST
#define DEC_AST

#include "ASTNode.hpp"
#include "ExprAST.hpp"

class DecAST : public ASTNode {

public:
	DecAST(boost::shared_ptr<SymbolTable> st, ExprAST* expr);
};

#endif