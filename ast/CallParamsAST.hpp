#ifndef CALL_PARARMS_AST
#define CALL_PARARMS_AST

#include "ASTNode.hpp"
#include <antlr3.h>
#include <vector>
#include "../idents/Type.hpp"
#include "ExprAST.hpp"

class CallParamsAST : public ASTNode {
	boost::shared_ptr<SymbolTable> _st;
	pANTLR3_BASE_TREE _tree;
	vector< boost::shared_ptr<Type> > _types;
public:
	CallParamsAST(boost::shared_ptr<SymbolTable> st, pANTLR3_BASE_TREE tree);
	pANTLR3_BASE_TREE childByNum(pANTLR3_BASE_TREE tree, int num);
	vector< boost::shared_ptr<Type> > getTypes();
};

#endif