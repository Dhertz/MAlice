#ifndef CALL_PARARMS_AST
#define CALL_PARARMS_AST

#include "ASTNode.hpp"
#include <antlr3.h>
#include <vector>
#include "../idents/Type.hpp"
#include "ExprAST.hpp"

class CallParamsAST : public ASTNode {
	SymbolTable* _st;
	pANTLR3_BASE_TREE _tree;
	vector<Type*> _types;
public:
	CallParamsAST(SymbolTable* st, pANTLR3_BASE_TREE tree);
	pANTLR3_BASE_TREE childByNum(pANTLR3_BASE_TREE tree, int num);
	vector<Type*> getTypes();
};

#endif