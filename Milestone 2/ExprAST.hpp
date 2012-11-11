#ifndef EXPR_AST_H
#define EXPR_AST_H

#include "NullType.hpp"

class ExprAST {

public:
	ExprAST();
	Type check();

};

#endif