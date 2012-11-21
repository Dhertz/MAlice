#ifndef EXPR_AST_H
#define EXPR_AST_H

#include <antlr3.h>
#include <set>
#include <vector>
#include "ASTNode.hpp"
#include "CallParamsAST.hpp"
#include "FuncAST.hpp"
#include "../idents/Type.hpp"
#include "../idents/Function.hpp"

class ExprAST : public ASTNode {
	set<string> _boolArgBoolRet;
	set<string> _intArgIntRet;
	set<string> _boolArgsBoolRet;
	set<string> _mixedArgsMixedRet;
	set<string> _mixedArgsBoolRet;
	boost::shared_ptr<Type> _type;
	pANTLR3_BASE_TREE _tree;
	pANTLR3_BASE_TREE childByNum(pANTLR3_BASE_TREE tree, int num);
	string createStringFromTree(pANTLR3_BASE_TREE tree);
	boost::shared_ptr<Type> recurseTree(pANTLR3_BASE_TREE tree, string expectedType);
public:
	ExprAST(boost::shared_ptr<SymbolTable> st, pANTLR3_BASE_TREE tree, boost::shared_ptr<ASTNode> parent);
	void check();
	boost::shared_ptr<Type> getTypeName();
};

#endif
