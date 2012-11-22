#ifndef CALL_PARARMS_AST
#define CALL_PARARMS_AST

#include "ASTNode.hpp"
#include <antlr3.h>
#include <vector>
#include "../idents/Type.hpp"
#include "ExprAST.hpp"
#include "../TreeUtils.hpp"

class CallParamsAST : public ASTNode {
	boost::shared_ptr<SymbolTable> _st;
	pANTLR3_BASE_TREE _tree;
	vector< boost::shared_ptr<Type> > _types;
    int _lineNo;
public:
	CallParamsAST(boost::shared_ptr<SymbolTable> st, pANTLR3_BASE_TREE tree, boost::shared_ptr<ASTNode> parent, int lineNo);
	vector< boost::shared_ptr<Type> > getTypes();
};

#endif
